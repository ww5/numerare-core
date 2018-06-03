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

#include "crypto/hash.h"
#include "cryptonote_basic/cryptonote_basic.h"
#include "ringct/rctSigs.h"

#include <unordered_map>
#include <vector>

namespace cryptonote
{

namespace rpc
{

  struct block_with_transactions
  {
    cryptonote::block block;
    std::unordered_map<crypto::hash, cryptonote::transaction> transactions;
  };

  typedef std::vector<uint64_t> tx_output_indices;

  typedef std::vector<tx_output_indices> block_output_indices;

  struct transaction_info
  {
    cryptonote::transaction transaction;
    bool in_pool;
    uint64_t height;
  };

  struct output_key_and_amount_index
  {
    uint64_t amount_index;
    crypto::public_key key;
  };

  typedef std::vector<output_key_and_amount_index> outputs_for_amount;

  struct amount_with_random_outputs
  {
    uint64_t amount;
    outputs_for_amount outputs;
  };

  struct peer
  {
    uint64_t id;
    uint32_t ip;
    uint16_t port;
    uint64_t last_seen;
  };

  struct tx_in_pool
  {
    cryptonote::transaction tx;
    crypto::hash tx_hash;
    uint64_t blob_size;
    uint64_t fee;
    crypto::hash max_used_block_hash;
    uint64_t max_used_block_height;
    bool kept_by_block;
    crypto::hash last_failed_block_hash;
    uint64_t last_failed_block_height;
    uint64_t receive_time;
    uint64_t last_relayed_time;
    bool relayed;
    bool do_not_relay;
    bool double_spend_seen;
  };

  typedef std::unordered_map<crypto::key_image, std::vector<crypto::hash> > key_images_with_tx_hashes;

  struct output_amount_count
  {
    uint64_t amount;
    uint64_t total_count;
    uint64_t unlocked_count;
    uint64_t recent_count;
  };

  struct output_amount_and_index
  {
    uint64_t amount;
    uint64_t index;
  };

  struct output_key_mask_unlocked
  {
    crypto::public_key key;
    rct::key mask;
    bool unlocked;
  };

  struct hard_fork_info
  {
    uint8_t version;
    bool enabled;
    uint32_t window;
    uint32_t votes;
    uint32_t threshold;
    uint8_t voting;
    uint32_t state;
    uint64_t earliest_height;
  };

  //required by JSON-RPC 2.0 spec
  struct error
  {
    // not really using code, maybe later.
    error() : use(false), code(1) { }

    bool use;  // do not serialize

    int32_t code;

    // not required by spec, but int error codes aren't perfect
    std::string error_str;

    std::string message;

    //TODO: data member?  not required, may want later.
  };

  struct BlockHeaderResponse
  {
    uint64_t major_version;
    uint64_t minor_version;
    uint64_t timestamp;
    crypto::hash  prev_id;
    uint32_t nonce;
    uint64_t height;
    uint64_t depth;
    crypto::hash hash;
    uint64_t difficulty;
    uint64_t reward;
  };

  struct DaemonInfo
  {
    uint64_t height;
    uint64_t target_height;
    uint64_t difficulty;
    uint64_t target;
    uint64_t tx_count;
    uint64_t tx_pool_size;
    uint64_t alt_blocks_count;
    uint64_t outgoing_connections_count;
    uint64_t incoming_connections_count;
    uint64_t white_peerlist_size;
    uint64_t grey_peerlist_size;
    bool mainnet;
    bool testnet;
    bool stagenet;
    crypto::hash top_block_hash;
    uint64_t cumulative_difficulty;
    uint64_t block_size_limit;
    uint64_t start_time;
  };

}  // namespace rpc

}  // namespace cryptonote
