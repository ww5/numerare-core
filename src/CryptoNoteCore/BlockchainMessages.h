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

#include <functional>
#include <vector>

#include <CryptoNote.h>

namespace CryptoNote {

namespace Messages {
// immutable messages
struct NewBlock {
  uint32_t blockIndex;
  Crypto::Hash blockHash;
};

struct NewAlternativeBlock {
  uint32_t blockIndex;
  Crypto::Hash blockHash;
};

struct ChainSwitch {
  uint32_t commonRootIndex;
  std::vector<Crypto::Hash> blocksFromCommonRoot;
};

struct AddTransaction {
  std::vector<Crypto::Hash> hashes;
};

struct DeleteTransaction {
  std::vector<Crypto::Hash> hashes;
  enum class Reason {
    InBlock,
    Outdated,
    NotActual
  } reason;
};
}

class BlockchainMessage {
public:
  enum class Type {
    NewBlock,
    NewAlternativeBlock,
    ChainSwitch,
    AddTransaction,
    DeleteTransaction
  };

  using NewBlock = Messages::NewBlock;
  using NewAlternativeBlock = Messages::NewAlternativeBlock;
  using ChainSwitch = Messages::ChainSwitch;
  using AddTransaction = Messages::AddTransaction;
  using DeleteTransaction = Messages::DeleteTransaction;

  BlockchainMessage(const NewBlock& message);
  BlockchainMessage(const NewAlternativeBlock& message);
  BlockchainMessage(const ChainSwitch& message);
  BlockchainMessage(const AddTransaction& message);
  BlockchainMessage(const DeleteTransaction& message);

  BlockchainMessage(const BlockchainMessage& other);

  ~BlockchainMessage();

  // pattern matchin API
  void match(std::function<void(const NewBlock&)>, std::function<void(const NewAlternativeBlock&)>,
             std::function<void(const ChainSwitch&)>, std::function<void(const AddTransaction&)>,
             std::function<void(const DeleteTransaction&)>) const;

  // API with explicit type handling
  Type getType() const;
  const NewBlock& getNewBlock() const;
  const NewAlternativeBlock& getNewAlternativeBlock() const;
  const ChainSwitch& getChainSwitch() const;
  const AddTransaction& getAddTransaction() const;
  const DeleteTransaction& getDeleteTransaction() const;

private:
  const Type type;
  union {
    NewBlock newBlock;
    NewAlternativeBlock newAlternativeBlock;
    ChainSwitch* chainSwitch;
    AddTransaction* addTransaction;
    DeleteTransaction* deleteTransaction;
  };
};

// factory functions
BlockchainMessage makeChainSwitchMessage(uint32_t index, std::vector<Crypto::Hash>&& hashes);
BlockchainMessage makeNewAlternativeBlockMessage(uint32_t index, const Crypto::Hash& hash);
BlockchainMessage makeNewBlockMessage(uint32_t index, const Crypto::Hash& hash);
BlockchainMessage makeAddTransactionMessage(std::vector<Crypto::Hash>&& hash);
BlockchainMessage makeDelTransactionMessage(std::vector<Crypto::Hash>&& hash, Messages::DeleteTransaction::Reason r);
}
