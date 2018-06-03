/***
	MIT License

	Copyright (c) 2018 NUMERARE

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	Parts of this file are originally Copyright (c) 2012-2014 The CryptoNote developers,
	                                                2014-2018 The Monero Project
***/

#include <unordered_set>
#include "include_base_utils.h"
#include "crypto/crypto.h"
#include "ringct/rctOps.h"
#include "cryptonote_basic/account.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "multisig.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "multisig"

using namespace std;

static const rct::key multisig_salt = { {'M', 'u', 'l', 't' , 'i', 's', 'i', 'g', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

namespace cryptonote
{
  //-----------------------------------------------------------------
  crypto::secret_key get_multisig_blinded_secret_key(const crypto::secret_key &key)
  {
    rct::keyV data;
    data.push_back(rct::sk2rct(key));
    data.push_back(multisig_salt);
    return rct::rct2sk(rct::hash_to_scalar(data));
  }
  //-----------------------------------------------------------------
  void generate_multisig_N_N(const account_keys &keys, const std::vector<crypto::public_key> &spend_keys, std::vector<crypto::secret_key> &multisig_keys, rct::key &spend_skey, rct::key &spend_pkey)
  {
    // the multisig spend public key is the sum of all spend public keys
    multisig_keys.clear();
    const crypto::secret_key spend_secret_key = get_multisig_blinded_secret_key(keys.m_spend_secret_key);
    CHECK_AND_ASSERT_THROW_MES(crypto::secret_key_to_public_key(spend_secret_key, (crypto::public_key&)spend_pkey), "Failed to derive public key");
    for (const auto &k: spend_keys)
      rct::addKeys(spend_pkey, spend_pkey, rct::pk2rct(k));
    multisig_keys.push_back(spend_secret_key);
    spend_skey = rct::sk2rct(spend_secret_key);
  }
  //-----------------------------------------------------------------
  void generate_multisig_N1_N(const account_keys &keys, const std::vector<crypto::public_key> &spend_keys, std::vector<crypto::secret_key> &multisig_keys, rct::key &spend_skey, rct::key &spend_pkey)
  {
    multisig_keys.clear();
    spend_pkey = rct::identity();
    spend_skey = rct::zero();

    // create all our composite private keys
    crypto::secret_key blinded_skey = get_multisig_blinded_secret_key(keys.m_spend_secret_key);
    for (const auto &k: spend_keys)
    {
      rct::key sk = rct::scalarmultKey(rct::pk2rct(k), rct::sk2rct(blinded_skey));
      crypto::secret_key msk = get_multisig_blinded_secret_key(rct::rct2sk(sk));
      multisig_keys.push_back(msk);
      sc_add(spend_skey.bytes, spend_skey.bytes, (const unsigned char*)msk.data);
    }
  }
  //-----------------------------------------------------------------
  crypto::secret_key generate_multisig_view_secret_key(const crypto::secret_key &skey, const std::vector<crypto::secret_key> &skeys)
  {
    rct::key view_skey = rct::sk2rct(get_multisig_blinded_secret_key(skey));
    for (const auto &k: skeys)
      sc_add(view_skey.bytes, view_skey.bytes, rct::sk2rct(k).bytes);
    return rct::rct2sk(view_skey);
  }
  //-----------------------------------------------------------------
  crypto::public_key generate_multisig_N1_N_spend_public_key(const std::vector<crypto::public_key> &pkeys)
  {
    rct::key spend_public_key = rct::identity();
    for (const auto &pk: pkeys)
    {
      rct::addKeys(spend_public_key, spend_public_key, rct::pk2rct(pk));
    }
    return rct::rct2pk(spend_public_key);
  }
  //-----------------------------------------------------------------
  bool generate_multisig_key_image(const account_keys &keys, size_t multisig_key_index, const crypto::public_key& out_key, crypto::key_image& ki)
  {
    if (multisig_key_index >= keys.m_multisig_keys.size())
      return false;
    crypto::generate_key_image(out_key, keys.m_multisig_keys[multisig_key_index], ki);
    return true;
  }
  //-----------------------------------------------------------------
  void generate_multisig_LR(const crypto::public_key pkey, const crypto::secret_key &k, crypto::public_key &L, crypto::public_key &R)
  {
    rct::scalarmultBase((rct::key&)L, rct::sk2rct(k));
    crypto::generate_key_image(pkey, k, (crypto::key_image&)R);
  }
  //-----------------------------------------------------------------
  bool generate_multisig_composite_key_image(const account_keys &keys, const std::unordered_map<crypto::public_key, subaddress_index>& subaddresses, const crypto::public_key& out_key, const crypto::public_key &tx_public_key, const std::vector<crypto::public_key>& additional_tx_public_keys, size_t real_output_index, const std::vector<crypto::key_image> &pkis, crypto::key_image &ki)
  {
    cryptonote::keypair in_ephemeral;
    if (!cryptonote::generate_key_image_helper(keys, subaddresses, out_key, tx_public_key, additional_tx_public_keys, real_output_index, in_ephemeral, ki, keys.get_device()))
      return false;
    std::unordered_set<crypto::key_image> used;
    for (size_t m = 0; m < keys.m_multisig_keys.size(); ++m)
    {
      crypto::key_image pki;
      bool r = cryptonote::generate_multisig_key_image(keys, m, out_key, pki);
      if (!r)
        return false;
      used.insert(pki);
    }
    for (const auto &pki: pkis)
    {
      if (used.find(pki) == used.end())
      {
        used.insert(pki);
        rct::addKeys((rct::key&)ki, rct::ki2rct(ki), rct::ki2rct(pki));
      }
    }
    return true;
  }
  //-----------------------------------------------------------------
}
