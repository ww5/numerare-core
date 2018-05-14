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

	Parts of this file are originally Copyright (c) 2012-2017 The CryptoNote developers, The Bytecoin developers
***/

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace CryptoNote {
	namespace parameters {
		const uint32_t CRYPTONOTE_MAX_BLOCK_NUMBER                   = 500000000;
		const size_t   CRYPTONOTE_MAX_BLOCK_BLOB_SIZE                = 500000000;
		const size_t   CRYPTONOTE_MAX_TX_SIZE                        = 1000000000;
		const uint64_t CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX       = 13;
		const uint32_t CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW          = 60;
		
		const size_t   BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW             = 60;

		// MONEY_SUPPLY - total number coins to be generated
		const uint64_t MONEY_SUPPLY                                  = static_cast<uint64_t>(-1);
		const size_t MIN_MIXIN                          			 = 0;
		const uint8_t MANDATORY_MIXIN_BLOCK_VERSION     			 = 0;
		const uint32_t MIXIN_START_HEIGHT                            = 0;
		const uint32_t MANDATORY_TRANSACTION                         = 0;
		const uint32_t KILL_HEIGHT                          		 = 0;
		const uint64_t TAIL_EMISSION_REWARD                          = 0;
		const size_t CRYPTONOTE_COIN_VERSION                         = 0;
		const uint32_t ZAWY_DIFFICULTY_BLOCK_INDEX                   = 0;
		const uint32_t ZAWY_DIFFICULTY_LAST_BLOCK                    = 0;
		const uint32_t ZAWY_LWMA_DIFFICULTY_BLOCK_INDEX              = 0;
		const uint32_t ZAWY_LWMA_DIFFICULTY_LAST_BLOCK               = 0;
		const size_t ZAWY_LWMA_DIFFICULTY_N                          = 0;
		const uint32_t BUGGED_ZAWY_DIFFICULTY_BLOCK_INDEX            = 0;
		const unsigned EMISSION_SPEED_FACTOR                         = 18;
		const uint64_t GENESIS_BLOCK_REWARD                          = 0;
		static_assert(EMISSION_SPEED_FACTOR <= 8 * sizeof(uint64_t), "Bad EMISSION_SPEED_FACTOR");

		const size_t   CRYPTONOTE_REWARD_BLOCKS_WINDOW               = 100;
		const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE     = 100000; //size of block (bytes) after which reward for block calculated using block size
		const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2  = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE;
		const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1  = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE;
		const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_CURRENT = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE;
		const size_t   CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE        = 600;
		const size_t   CRYPTONOTE_DISPLAY_DECIMAL_POINT              = 3;
		const uint64_t MINIMUM_FEE                                   = UINT64_C(0);    // pow(10, 6)
		const uint64_t DEFAULT_DUST_THRESHOLD                        = UINT64_C(0);    // pow(10, 6)
		// Use 0 for default max transaction size limit
		const uint64_t MAX_TRANSACTION_SIZE_LIMIT                    = 5;
		const uint64_t DEFAULT_FEE                                   = MINIMUM_FEE;

		const uint64_t DIFFICULTY_TARGET                             = 60; // seconds
		const uint64_t EXPECTED_NUMBER_OF_BLOCKS_PER_DAY             = 24 * 60; //1 block per minute
		const size_t   DIFFICULTY_WINDOW                             = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY; // blocks
		const size_t   DIFFICULTY_WINDOW_V1                          = DIFFICULTY_WINDOW;
		const size_t   DIFFICULTY_WINDOW_V2                          = DIFFICULTY_WINDOW;
		const size_t   DIFFICULTY_CUT                                = 60;  // timestamps to cut after sorting
		const size_t   DIFFICULTY_CUT_V1                             = DIFFICULTY_CUT;
		const size_t   DIFFICULTY_CUT_V2                             = DIFFICULTY_CUT;
		const size_t   DIFFICULTY_LAG                                = 15;  // !!!
		const size_t   DIFFICULTY_LAG_V1                             = DIFFICULTY_LAG;
		const size_t   DIFFICULTY_LAG_V2                             = DIFFICULTY_LAG;
		static_assert(2 * DIFFICULTY_CUT <= DIFFICULTY_WINDOW - 2, "Bad DIFFICULTY_WINDOW or DIFFICULTY_CUT");
		
		const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT            = 60 * DIFFICULTY_TARGET;

		const size_t   MAX_BLOCK_SIZE_INITIAL                        = 100 * 1024;
		const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR         = 100 * 1024;
		const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_DENOMINATOR       = 365 * 24 * 60 * 60 / DIFFICULTY_TARGET;

		const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS     = 1;
		const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS    = DIFFICULTY_TARGET * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS;

		const uint64_t CRYPTONOTE_MEMPOOL_TX_LIVETIME                = 60 * 60 * 24;     //seconds, one day
		const uint64_t CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME = 60 * 60 * 24 * 7; //seconds, one week
		const uint64_t CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL = 7;  // CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL * CRYPTONOTE_MEMPOOL_TX_LIVETIME = time to forget tx

		const size_t   FUSION_TX_MAX_SIZE                            = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_CURRENT * 30 / 100;
		const size_t   FUSION_TX_MIN_INPUT_COUNT                     = 12;
		const size_t   FUSION_TX_MIN_IN_OUT_COUNT_RATIO              = 4;

		const uint32_t KEY_IMAGE_CHECKING_BLOCK_INDEX                = 0;
		const uint32_t UPGRADE_HEIGHT_V2                             = 546602;
		const uint32_t UPGRADE_HEIGHT_V3                             = 985548;
		const unsigned UPGRADE_VOTING_THRESHOLD                      = 90;               // percent
		const uint32_t UPGRADE_VOTING_WINDOW                         = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY;  // blocks
		const uint32_t UPGRADE_WINDOW                                = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY;  // blocks
		static_assert(0 < UPGRADE_VOTING_THRESHOLD && UPGRADE_VOTING_THRESHOLD <= 100, "Bad UPGRADE_VOTING_THRESHOLD");
		static_assert(UPGRADE_VOTING_WINDOW > 1, "Bad UPGRADE_VOTING_WINDOW");

		const char     CRYPTONOTE_BLOCKS_FILENAME[]                  = "blocks.list";
		const char     CRYPTONOTE_BLOCKINDEXES_FILENAME[]            = "indexes.list";
		const char     CRYPTONOTE_POOLDATA_FILENAME[]                = "poolstatus.data";
		const char     P2P_NET_DATA_FILENAME[]                       = "p2pstatus.data";
		const char     MINER_CONFIG_FILE_NAME[]                      = "miner-conf.json";
		const char     GENESIS_COINBASE_TX_HEX[]                     = "013c01ff000180b0def7d32b02c1e644dbbf00b6b6d558613bf1dffa62586abd1d9b023bfcca1be5512b738bf52101d908a7de1955d5cb14aca6f49c2aa10dc0eaa459c42671ea17278f7ed7816caf";
	} // parameters

	const char     CRYPTONOTE_NAME[]                             = "numerare";

	const uint8_t  TRANSACTION_VERSION_1                         =  1;
	const uint8_t  TRANSACTION_VERSION_2                         =  2;
	const uint8_t  CURRENT_TRANSACTION_VERSION                   =  TRANSACTION_VERSION_1;
	const uint8_t  BLOCK_MAJOR_VERSION_1                         =  1;
	const uint8_t  BLOCK_MAJOR_VERSION_2                         =  2;
	const uint8_t  BLOCK_MAJOR_VERSION_3                         =  3;
	const uint8_t  BLOCK_MINOR_VERSION_0                         =  0;
	const uint8_t  BLOCK_MINOR_VERSION_1                         =  1;

	const size_t   BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT        =  10000;  //by default, blocks ids count in synchronizing
	const size_t   BLOCKS_SYNCHRONIZING_DEFAULT_COUNT            =  100;    //by default, blocks count in blocks downloading
	const size_t   COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT         =  1000;

	const int      P2P_DEFAULT_PORT                              =  19131;
	const int      RPC_DEFAULT_PORT                              =  13191;

	const size_t   P2P_LOCAL_WHITE_PEERLIST_LIMIT                =  1000;
	const size_t   P2P_LOCAL_GRAY_PEERLIST_LIMIT                 =  5000;

	const size_t   P2P_CONNECTION_MAX_WRITE_BUFFER_SIZE          = 32 * 1024 * 1024; // 32 MB
	const uint32_t P2P_DEFAULT_CONNECTIONS_COUNT                 = 8;
	const size_t   P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT     = 70;
	const uint32_t P2P_DEFAULT_HANDSHAKE_INTERVAL                = 60;            // seconds
	const uint32_t P2P_DEFAULT_PACKET_MAX_SIZE                   = 50000000;      // 50000000 bytes maximum packet size
	const uint32_t P2P_DEFAULT_PEERS_IN_HANDSHAKE                = 250;
	const uint32_t P2P_DEFAULT_CONNECTION_TIMEOUT                = 5000;          // 5 seconds
	const uint32_t P2P_DEFAULT_PING_CONNECTION_TIMEOUT           = 2000;          // 2 seconds
	const uint64_t P2P_DEFAULT_INVOKE_TIMEOUT                    = 60 * 2 * 1000; // 2 minutes
	const size_t   P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT          = 5000;          // 5 seconds

	const uint32_t  P2P_FAILED_ADDR_FORGET_SECONDS               = (60 * 60);     //1 hour
	const uint32_t  P2P_IP_BLOCKTIME                             = (60 * 60 * 24);  //24 hour
	const uint32_t  P2P_IP_FAILS_BEFORE_BLOCK                    = 10;
	const uint32_t  P2P_IDLE_CONNECTION_KILL_INTERVAL            = (5 * 60); //5 minutes

	const char     P2P_STAT_TRUSTED_PUB_KEY[]                    = "2996a7a2a6fa63ee86768513635ba2c820bcc7a550521819228d675840e08624";

	const char* const SEED_NODES[] = {""};

	struct CheckpointData {
	  uint32_t index;
	  const char* blockId;
	};

	const std::initializer_list<CheckpointData> CHECKPOINTS = {
	  {0, "3992958c861c2f8c7266b16f548411ac0c28f8331ccd9bb3bc96d65302d03b22"},
	  {9, "05db78aee53b6806c88495609698935f98641822c2f9b5e9ef42ab936a5bc8ff"},
	  {999, "03a7b70d6c432bda50d21c4414534d9c262ccba9fe59eeb16c16a63478ee4bce"},
	  {4999, "e041b036296552f99036e52c19cca9d4a5819993a1ffb1878d1b335fa3b80bc1"},
	  {9999, "4b5839af7bae1ca0f0d532d987e2a291051cc282040461c1ac316418574100e5"}
	};
} // CryptoNote

#define ALLOW_DEBUG_COMMANDS