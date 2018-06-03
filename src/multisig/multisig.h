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

#pragma once

#include <vector>
#include <unordered_map>
#include "crypto/crypto.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "ringct/rctTypes.h"

namespace cryptonote
{
  struct account_keys;

  crypto::secret_key get_multisig_blinded_secret_key(const crypto::secret_key &key);
  void generate_multisig_N_N(const account_keys &keys, const std::vector<crypto::public_key> &spend_keys, std::vector<crypto::secret_key> &multisig_keys, rct::key &spend_skey, rct::key &spend_pkey);
  void generate_multisig_N1_N(const account_keys &keys, const std::vector<crypto::public_key> &spend_keys, std::vector<crypto::secret_key> &multisig_keys, rct::key &spend_skey, rct::key &spend_pkey);
  crypto::secret_key generate_multisig_view_secret_key(const crypto::secret_key &skey, const std::vector<crypto::secret_key> &skeys);
  crypto::public_key generate_multisig_N1_N_spend_public_key(const std::vector<crypto::public_key> &pkeys);
  bool generate_multisig_key_image(const account_keys &keys, size_t multisig_key_index, const crypto::public_key& out_key, crypto::key_image& ki);
  void generate_multisig_LR(const crypto::public_key pkey, const crypto::secret_key &k, crypto::public_key &L, crypto::public_key &R);
  bool generate_multisig_composite_key_image(const account_keys &keys, const std::unordered_map<crypto::public_key, cryptonote::subaddress_index>& subaddresses, const crypto::public_key& out_key, const crypto::public_key &tx_public_key, const std::vector<crypto::public_key>& additional_tx_public_keys, size_t real_output_index, const std::vector<crypto::key_image> &pkis, crypto::key_image &ki);
}
