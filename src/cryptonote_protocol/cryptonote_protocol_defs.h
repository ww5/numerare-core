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

#include <list>
#include "serialization/keyvalue_serialization.h"
#include "cryptonote_basic/cryptonote_basic.h"
#include "cryptonote_basic/blobdatatype.h"
namespace cryptonote
{


#define BC_COMMANDS_POOL_BASE 2000

  /************************************************************************/
  /* P2P connection info, serializable to json                            */
  /************************************************************************/
  struct connection_info
  {
    bool incoming;
    bool localhost;
    bool local_ip;

    std::string address;
    std::string host;
    std::string ip;
    std::string port;

    std::string peer_id;

    uint64_t recv_count;
    uint64_t recv_idle_time;

    uint64_t send_count;
    uint64_t send_idle_time;

    std::string state;

    uint64_t live_time;

	uint64_t avg_download;
	uint64_t current_download;
	
	uint64_t avg_upload;
	uint64_t current_upload;
  
	uint32_t support_flags;

	std::string connection_id;

    uint64_t height;

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(incoming)
      KV_SERIALIZE(localhost)
      KV_SERIALIZE(local_ip)
      KV_SERIALIZE(address)
      KV_SERIALIZE(host)
      KV_SERIALIZE(ip)
      KV_SERIALIZE(port)
      KV_SERIALIZE(peer_id)
      KV_SERIALIZE(recv_count)
      KV_SERIALIZE(recv_idle_time)
      KV_SERIALIZE(send_count)
      KV_SERIALIZE(send_idle_time)
      KV_SERIALIZE(state)
      KV_SERIALIZE(live_time)
      KV_SERIALIZE(avg_download)
      KV_SERIALIZE(current_download)
      KV_SERIALIZE(avg_upload)
      KV_SERIALIZE(current_upload)
      KV_SERIALIZE(support_flags)
      KV_SERIALIZE(connection_id)
      KV_SERIALIZE(height)
    END_KV_SERIALIZE_MAP()
  };

  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct block_complete_entry
  {
    blobdata block;
    std::list<blobdata> txs;
    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(block)
      KV_SERIALIZE(txs)
    END_KV_SERIALIZE_MAP()
  };


  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct NOTIFY_NEW_BLOCK
  {
    const static int ID = BC_COMMANDS_POOL_BASE + 1;

    struct request
    {
      block_complete_entry b;
      uint64_t current_blockchain_height;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(b)
        KV_SERIALIZE(current_blockchain_height)
      END_KV_SERIALIZE_MAP()
    };
  };

  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct NOTIFY_NEW_TRANSACTIONS
  {
    const static int ID = BC_COMMANDS_POOL_BASE + 2;

    struct request
    {
      std::list<blobdata>   txs;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(txs)
      END_KV_SERIALIZE_MAP()
    };
  };
  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct NOTIFY_REQUEST_GET_OBJECTS
  {
    const static int ID = BC_COMMANDS_POOL_BASE + 3;

    struct request
    {
      std::list<crypto::hash>    txs;
      std::list<crypto::hash>    blocks;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(txs)
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(blocks)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct NOTIFY_RESPONSE_GET_OBJECTS
  {
    const static int ID = BC_COMMANDS_POOL_BASE + 4;

    struct request
    {
      std::list<blobdata>              txs;
      std::list<block_complete_entry>  blocks;
      std::list<crypto::hash>               missed_ids;
      uint64_t                         current_blockchain_height;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(txs)
        KV_SERIALIZE(blocks)
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(missed_ids)
        KV_SERIALIZE(current_blockchain_height)
      END_KV_SERIALIZE_MAP()
    };
  };


  struct CORE_SYNC_DATA
  {
    uint64_t current_height;
    uint64_t cumulative_difficulty;
    crypto::hash  top_id;
    uint8_t top_version;

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(current_height)
      KV_SERIALIZE(cumulative_difficulty)
      KV_SERIALIZE_VAL_POD_AS_BLOB(top_id)
      KV_SERIALIZE_OPT(top_version, (uint8_t)0)
    END_KV_SERIALIZE_MAP()
  };

  struct NOTIFY_REQUEST_CHAIN
  {
    const static int ID = BC_COMMANDS_POOL_BASE + 6;

    struct request
    {
      std::list<crypto::hash> block_ids; /*IDs of the first 10 blocks are sequential, next goes with pow(2,n) offset, like 2, 4, 8, 16, 32, 64 and so on, and the last one is always genesis block */

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(block_ids)
      END_KV_SERIALIZE_MAP()
    };
  };

  struct NOTIFY_RESPONSE_CHAIN_ENTRY
  {
    const static int ID = BC_COMMANDS_POOL_BASE + 7;

    struct request
    {
      uint64_t start_height;
      uint64_t total_height;
      uint64_t cumulative_difficulty;
      std::list<crypto::hash> m_block_ids;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(start_height)
        KV_SERIALIZE(total_height)
        KV_SERIALIZE(cumulative_difficulty)
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(m_block_ids)
      END_KV_SERIALIZE_MAP()
    };
  };
  
  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct NOTIFY_NEW_FLUFFY_BLOCK
  {
    const static int ID = BC_COMMANDS_POOL_BASE + 8;

    struct request
    {
      block_complete_entry b;
      uint64_t current_blockchain_height;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(b)
        KV_SERIALIZE(current_blockchain_height)
      END_KV_SERIALIZE_MAP()
    };
  };  

  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  struct NOTIFY_REQUEST_FLUFFY_MISSING_TX
  {
    const static int ID = BC_COMMANDS_POOL_BASE + 9;

    struct request
    {
      crypto::hash block_hash;
      uint64_t current_blockchain_height;      
      std::vector<uint64_t> missing_tx_indices;
      
      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE_VAL_POD_AS_BLOB(block_hash)
        KV_SERIALIZE(current_blockchain_height)
        KV_SERIALIZE_CONTAINER_POD_AS_BLOB(missing_tx_indices)
      END_KV_SERIALIZE_MAP()
    };
  }; 
    
}
