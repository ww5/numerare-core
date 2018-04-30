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

#include <vector>
#include <unordered_map>

#include "CryptoNoteCore/Account.h"
#include "CryptoNoteCore/CryptoNoteBasic.h"
#include "CryptoNoteCore/Currency.h"
#include "crypto/hash.h"

#include "../TestGenerator/TestGenerator.h"

class TestBlockchainGenerator
{
public:
  TestBlockchainGenerator(const CryptoNote::Currency& currency);

  //TODO: get rid of this method
  std::vector<CryptoNote::BlockTemplate>& getBlockchain();
  std::vector<CryptoNote::BlockTemplate> getBlockchainCopy();
  void generateEmptyBlocks(size_t count);
  bool getBlockRewardForAddress(const CryptoNote::AccountPublicAddress& address);
  bool generateTransactionsInOneBlock(const CryptoNote::AccountPublicAddress& address, size_t n);
  bool getSingleOutputTransaction(const CryptoNote::AccountPublicAddress& address, uint64_t amount);
  void addTxToBlockchain(const CryptoNote::Transaction& transaction);
  bool getTransactionByHash(const Crypto::Hash& hash, CryptoNote::Transaction& tx, bool checkTxPool = false);
  CryptoNote::Transaction getTransactionByHash(const Crypto::Hash& hash, bool checkTxPool = false);
  const CryptoNote::AccountBase& getMinerAccount() const;
  bool generateFromBaseTx(const CryptoNote::AccountBase& address);

  void putTxToPool(const CryptoNote::Transaction& tx);
  void getPoolSymmetricDifference(std::vector<Crypto::Hash>&& known_pool_tx_ids, Crypto::Hash known_block_id, bool& is_bc_actual,
    std::vector<CryptoNote::Transaction>& new_txs, std::vector<Crypto::Hash>& deleted_tx_ids);
  void putTxPoolToBlockchain();
  void clearTxPool();

  void cutBlockchain(uint32_t height);

  bool getTransactionGlobalIndexesByHash(const Crypto::Hash& transactionHash, std::vector<uint32_t>& globalIndexes);
  size_t getGeneratedTransactionsNumber(uint32_t index);
  void setMinerAccount(const CryptoNote::AccountBase& account);

private:
  struct KeyOutEntry {
    Crypto::Hash transactionHash;
    uint16_t indexOut;
  };
  
  void addGenesisBlock();
  void addMiningBlock();

  const CryptoNote::Currency& m_currency;
  test_generator generator;
  CryptoNote::AccountBase miner_acc;
  std::vector<CryptoNote::BlockTemplate> m_blockchain;
  std::unordered_map<Crypto::Hash, CryptoNote::Transaction> m_txs;
  std::unordered_map<Crypto::Hash, std::vector<uint32_t>> transactionGlobalOuts;
  std::unordered_map<uint64_t, std::vector<KeyOutEntry>> keyOutsIndex;

  std::unordered_map<Crypto::Hash, CryptoNote::Transaction> m_txPool;
  mutable std::mutex m_mutex;

  void addToBlockchain(const CryptoNote::Transaction& tx);
  void addToBlockchain(const std::vector<CryptoNote::Transaction>& txs);
  void addToBlockchain(const std::vector<CryptoNote::Transaction>& txs, const CryptoNote::AccountBase& minerAddress);
  void addTx(const CryptoNote::Transaction& tx);

  bool doGenerateTransactionsInOneBlock(CryptoNote::AccountPublicAddress const &address, size_t n);
};
