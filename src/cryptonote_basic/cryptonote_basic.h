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

#include <boost/variant.hpp>
#include <boost/functional/hash/hash.hpp>
#include <vector>
#include <cstring>  // memcmp
#include <sstream>
#include <atomic>
#include "serialization/variant.h"
#include "serialization/vector.h"
#include "serialization/binary_archive.h"
#include "serialization/json_archive.h"
#include "serialization/debug_archive.h"
#include "serialization/crypto.h"
#include "serialization/keyvalue_serialization.h" // eepe named serialization
#include "cryptonote_config.h"
#include "crypto/crypto.h"
#include "crypto/hash.h"
#include "misc_language.h"
#include "tx_extra.h"
#include "ringct/rctTypes.h"
#include "device/device.hpp"

namespace cryptonote
{
  typedef std::vector<crypto::signature> ring_signature;


  /* outputs */

  struct txout_to_script
  {
    std::vector<crypto::public_key> keys;
    std::vector<uint8_t> script;

    BEGIN_SERIALIZE_OBJECT()
      FIELD(keys)
      FIELD(script)
    END_SERIALIZE()
  };

  struct txout_to_scripthash
  {
    crypto::hash hash;
  };

  struct txout_to_key
  {
    txout_to_key() { }
    txout_to_key(const crypto::public_key &_key) : key(_key) { }
    crypto::public_key key;
  };


  /* inputs */

  struct txin_gen
  {
    size_t height;

    BEGIN_SERIALIZE_OBJECT()
      VARINT_FIELD(height)
    END_SERIALIZE()
  };

  struct txin_to_script
  {
    crypto::hash prev;
    size_t prevout;
    std::vector<uint8_t> sigset;

    BEGIN_SERIALIZE_OBJECT()
      FIELD(prev)
      VARINT_FIELD(prevout)
      FIELD(sigset)
    END_SERIALIZE()
  };

  struct txin_to_scripthash
  {
    crypto::hash prev;
    size_t prevout;
    txout_to_script script;
    std::vector<uint8_t> sigset;

    BEGIN_SERIALIZE_OBJECT()
      FIELD(prev)
      VARINT_FIELD(prevout)
      FIELD(script)
      FIELD(sigset)
    END_SERIALIZE()
  };

  struct txin_to_key
  {
    uint64_t amount;
    std::vector<uint64_t> key_offsets;
    crypto::key_image k_image;      // double spending protection

    BEGIN_SERIALIZE_OBJECT()
      VARINT_FIELD(amount)
      FIELD(key_offsets)
      FIELD(k_image)
    END_SERIALIZE()
  };


  typedef boost::variant<txin_gen, txin_to_script, txin_to_scripthash, txin_to_key> txin_v;

  typedef boost::variant<txout_to_script, txout_to_scripthash, txout_to_key> txout_target_v;

  //typedef std::pair<uint64_t, txout> out_t;
  struct tx_out
  {
    uint64_t amount;
    txout_target_v target;

    BEGIN_SERIALIZE_OBJECT()
      VARINT_FIELD(amount)
      FIELD(target)
    END_SERIALIZE()


  };

  class transaction_prefix
  {

  public:
    // tx information
    size_t   version;
    uint64_t unlock_time;  //number of block (or time), used as a limitation like: spend this tx not early then block/time

    std::vector<txin_v> vin;
    std::vector<tx_out> vout;
    //extra
    std::vector<uint8_t> extra;

    BEGIN_SERIALIZE()
      VARINT_FIELD(version)
      if(version == 0 || CURRENT_TRANSACTION_VERSION < version) return false;
      VARINT_FIELD(unlock_time)
      FIELD(vin)
      FIELD(vout)
      FIELD(extra)
    END_SERIALIZE()

  public:
    transaction_prefix(){}
  };

  class transaction: public transaction_prefix
  {
  private:
    // hash cash
    mutable std::atomic<bool> hash_valid;
    mutable std::atomic<bool> blob_size_valid;

  public:
    std::vector<std::vector<crypto::signature> > signatures; //count signatures  always the same as inputs count
    rct::rctSig rct_signatures;

    // hash cash
    mutable crypto::hash hash;
    mutable size_t blob_size;

    transaction();
    transaction(const transaction &t): transaction_prefix(t), hash_valid(false), blob_size_valid(false), signatures(t.signatures), rct_signatures(t.rct_signatures) { if (t.is_hash_valid()) { hash = t.hash; set_hash_valid(true); } if (t.is_blob_size_valid()) { blob_size = t.blob_size; set_blob_size_valid(true); } }
    transaction &operator=(const transaction &t) { transaction_prefix::operator=(t); set_hash_valid(false); set_blob_size_valid(false); signatures = t.signatures; rct_signatures = t.rct_signatures; if (t.is_hash_valid()) { hash = t.hash; set_hash_valid(true); } if (t.is_blob_size_valid()) { blob_size = t.blob_size; set_blob_size_valid(true); } return *this; }
    virtual ~transaction();
    void set_null();
    void invalidate_hashes();
    bool is_hash_valid() const { return hash_valid.load(std::memory_order_acquire); }
    void set_hash_valid(bool v) const { hash_valid.store(v,std::memory_order_release); }
    bool is_blob_size_valid() const { return blob_size_valid.load(std::memory_order_acquire); }
    void set_blob_size_valid(bool v) const { blob_size_valid.store(v,std::memory_order_release); }

    BEGIN_SERIALIZE_OBJECT()
      if (!typename Archive<W>::is_saving())
      {
        set_hash_valid(false);
        set_blob_size_valid(false);
      }

      FIELDS(*static_cast<transaction_prefix *>(this))

      if (version == 1)
      {
        ar.tag("signatures");
        ar.begin_array();
        PREPARE_CUSTOM_VECTOR_SERIALIZATION(vin.size(), signatures);
        bool signatures_not_expected = signatures.empty();
        if (!signatures_not_expected && vin.size() != signatures.size())
          return false;

        for (size_t i = 0; i < vin.size(); ++i)
        {
          size_t signature_size = get_signature_size(vin[i]);
          if (signatures_not_expected)
          {
            if (0 == signature_size)
              continue;
            else
              return false;
          }

          PREPARE_CUSTOM_VECTOR_SERIALIZATION(signature_size, signatures[i]);
          if (signature_size != signatures[i].size())
            return false;

          FIELDS(signatures[i]);

          if (vin.size() - i > 1)
            ar.delimit_array();
        }
        ar.end_array();
      }
      else
      {
        ar.tag("rct_signatures");
        if (!vin.empty())
        {
          ar.begin_object();
          bool r = rct_signatures.serialize_rctsig_base(ar, vin.size(), vout.size());
          if (!r || !ar.stream().good()) return false;
          ar.end_object();
          if (rct_signatures.type != rct::RCTTypeNull)
          {
            ar.tag("rctsig_prunable");
            ar.begin_object();
            r = rct_signatures.p.serialize_rctsig_prunable(ar, rct_signatures.type, vin.size(), vout.size(),
                vin.size() > 0 && vin[0].type() == typeid(txin_to_key) ? boost::get<txin_to_key>(vin[0]).key_offsets.size() - 1 : 0);
            if (!r || !ar.stream().good()) return false;
            ar.end_object();
          }
        }
      }
    END_SERIALIZE()

    template<bool W, template <bool> class Archive>
    bool serialize_base(Archive<W> &ar)
    {
      FIELDS(*static_cast<transaction_prefix *>(this))

      if (version == 1)
      {
      }
      else
      {
        ar.tag("rct_signatures");
        if (!vin.empty())
        {
          ar.begin_object();
          bool r = rct_signatures.serialize_rctsig_base(ar, vin.size(), vout.size());
          if (!r || !ar.stream().good()) return false;
          ar.end_object();
        }
      }
      return true;
    }

  private:
    static size_t get_signature_size(const txin_v& tx_in);
  };


  inline
  transaction::transaction()
  {
    set_null();
  }

  inline
  transaction::~transaction()
  {
    //set_null();
  }

  inline
  void transaction::set_null()
  {
    version = 1;
    unlock_time = 0;
    vin.clear();
    vout.clear();
    extra.clear();
    signatures.clear();
    rct_signatures.type = rct::RCTTypeNull;
    set_hash_valid(false);
    set_blob_size_valid(false);
  }

  inline
  void transaction::invalidate_hashes()
  {
    set_hash_valid(false);
    set_blob_size_valid(false);
  }

  inline
  size_t transaction::get_signature_size(const txin_v& tx_in)
  {
    struct txin_signature_size_visitor : public boost::static_visitor<size_t>
    {
      size_t operator()(const txin_gen& txin) const{return 0;}
      size_t operator()(const txin_to_script& txin) const{return 0;}
      size_t operator()(const txin_to_scripthash& txin) const{return 0;}
      size_t operator()(const txin_to_key& txin) const {return txin.key_offsets.size();}
    };

    return boost::apply_visitor(txin_signature_size_visitor(), tx_in);
  }



  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct block_header
  {
    uint8_t major_version;
    uint8_t minor_version;  // now used as a voting mechanism, rather than how this particular block is built
    uint64_t timestamp;
    uint64_t endtime;
    crypto::hash  prev_id;
    uint32_t nonce;

    BEGIN_SERIALIZE()
      VARINT_FIELD(major_version)
      VARINT_FIELD(minor_version)
      VARINT_FIELD(timestamp)
      VARINT_FIELD(endtime)
      FIELD(prev_id)
      FIELD(nonce)
    END_SERIALIZE()
  };

  struct block: public block_header
  {
  private:
    // hash cash
    mutable std::atomic<bool> hash_valid;

  public:
    block(): block_header(), hash_valid(false) {}
    block(const block &b): block_header(b), hash_valid(false), miner_tx(b.miner_tx), tx_hashes(b.tx_hashes) { if (b.is_hash_valid()) { hash = b.hash; set_hash_valid(true); } }
    block &operator=(const block &b) { block_header::operator=(b); hash_valid = false; miner_tx = b.miner_tx; tx_hashes = b.tx_hashes; if (b.is_hash_valid()) { hash = b.hash; set_hash_valid(true); } return *this; }
    void invalidate_hashes() { set_hash_valid(false); }
    bool is_hash_valid() const { return hash_valid.load(std::memory_order_acquire); }
    void set_hash_valid(bool v) const { hash_valid.store(v,std::memory_order_release); }

    transaction miner_tx;
    std::vector<crypto::hash> tx_hashes;

    // hash cash
    mutable crypto::hash hash;

    BEGIN_SERIALIZE_OBJECT()
      if (!typename Archive<W>::is_saving())
        set_hash_valid(false);

      FIELDS(*static_cast<block_header *>(this))
      FIELD(miner_tx)
      FIELD(tx_hashes)
    END_SERIALIZE()
  };


  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct account_public_address
  {
    crypto::public_key m_spend_public_key;
    crypto::public_key m_view_public_key;

    BEGIN_SERIALIZE_OBJECT()
      FIELD(m_spend_public_key)
      FIELD(m_view_public_key)
    END_SERIALIZE()

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE_VAL_POD_AS_BLOB_FORCE(m_spend_public_key)
      KV_SERIALIZE_VAL_POD_AS_BLOB_FORCE(m_view_public_key)
    END_KV_SERIALIZE_MAP()

    bool operator==(const account_public_address& rhs) const
    {
      return m_spend_public_key == rhs.m_spend_public_key &&
             m_view_public_key == rhs.m_view_public_key;
    }

    bool operator!=(const account_public_address& rhs) const
    {
      return !(*this == rhs);
    }
  };

  struct keypair
  {
    crypto::public_key pub;
    crypto::secret_key sec;

    static inline keypair generate(hw::device &hwdev)
    {
      keypair k;
      hwdev.generate_keys(k.pub, k.sec);
      return k;
    }
  };
  //---------------------------------------------------------------

}

namespace std {
  template <>
  struct hash<cryptonote::account_public_address>
  {
    std::size_t operator()(const cryptonote::account_public_address& addr) const
    {
      // https://stackoverflow.com/a/17017281
      size_t res = 17;
      res = res * 31 + hash<crypto::public_key>()(addr.m_spend_public_key);
      res = res * 31 + hash<crypto::public_key>()(addr.m_view_public_key);
      return res;
    }
  };
}

BLOB_SERIALIZER(cryptonote::txout_to_key);
BLOB_SERIALIZER(cryptonote::txout_to_scripthash);

VARIANT_TAG(binary_archive, cryptonote::txin_gen, 0xff);
VARIANT_TAG(binary_archive, cryptonote::txin_to_script, 0x0);
VARIANT_TAG(binary_archive, cryptonote::txin_to_scripthash, 0x1);
VARIANT_TAG(binary_archive, cryptonote::txin_to_key, 0x2);
VARIANT_TAG(binary_archive, cryptonote::txout_to_script, 0x0);
VARIANT_TAG(binary_archive, cryptonote::txout_to_scripthash, 0x1);
VARIANT_TAG(binary_archive, cryptonote::txout_to_key, 0x2);
VARIANT_TAG(binary_archive, cryptonote::transaction, 0xcc);
VARIANT_TAG(binary_archive, cryptonote::block, 0xbb);

VARIANT_TAG(json_archive, cryptonote::txin_gen, "gen");
VARIANT_TAG(json_archive, cryptonote::txin_to_script, "script");
VARIANT_TAG(json_archive, cryptonote::txin_to_scripthash, "scripthash");
VARIANT_TAG(json_archive, cryptonote::txin_to_key, "key");
VARIANT_TAG(json_archive, cryptonote::txout_to_script, "script");
VARIANT_TAG(json_archive, cryptonote::txout_to_scripthash, "scripthash");
VARIANT_TAG(json_archive, cryptonote::txout_to_key, "key");
VARIANT_TAG(json_archive, cryptonote::transaction, "tx");
VARIANT_TAG(json_archive, cryptonote::block, "block");

VARIANT_TAG(debug_archive, cryptonote::txin_gen, "gen");
VARIANT_TAG(debug_archive, cryptonote::txin_to_script, "script");
VARIANT_TAG(debug_archive, cryptonote::txin_to_scripthash, "scripthash");
VARIANT_TAG(debug_archive, cryptonote::txin_to_key, "key");
VARIANT_TAG(debug_archive, cryptonote::txout_to_script, "script");
VARIANT_TAG(debug_archive, cryptonote::txout_to_scripthash, "scripthash");
VARIANT_TAG(debug_archive, cryptonote::txout_to_key, "key");
VARIANT_TAG(debug_archive, cryptonote::transaction, "tx");
VARIANT_TAG(debug_archive, cryptonote::block, "block");
