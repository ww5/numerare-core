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

#include "TransactionPool.h"

#include "Common/int-util.h"
#include "CryptoNoteBasicImpl.h"
#include "CryptoNoteCore/TransactionExtra.h"

namespace CryptoNote {

// lhs > hrs
bool TransactionPool::TransactionPriorityComparator::operator()(const PendingTransactionInfo& lhs, const PendingTransactionInfo& rhs) const {
  const CachedTransaction& left = lhs.cachedTransaction;
  const CachedTransaction& right = rhs.cachedTransaction;

  // price(lhs) = lhs.fee / lhs.blobSize
  // price(lhs) > price(rhs) -->
  // lhs.fee / lhs.blobSize > rhs.fee / rhs.blobSize -->
  // lhs.fee * rhs.blobSize > rhs.fee * lhs.blobSize
  uint64_t lhs_hi, lhs_lo = mul128(left.getTransactionFee(), right.getTransactionBinaryArray().size(), &lhs_hi);
  uint64_t rhs_hi, rhs_lo = mul128(right.getTransactionFee(), left.getTransactionBinaryArray().size(), &rhs_hi);

  return
    // prefer more profitable transactions
    (lhs_hi >  rhs_hi) ||
    (lhs_hi == rhs_hi && lhs_lo >  rhs_lo) ||
    // prefer smaller
    (lhs_hi == rhs_hi && lhs_lo == rhs_lo && left.getTransactionBinaryArray().size() <  right.getTransactionBinaryArray().size()) ||
    // prefer older
    (lhs_hi == rhs_hi && lhs_lo == rhs_lo && left.getTransactionBinaryArray().size() == right.getTransactionBinaryArray().size() && lhs.receiveTime < rhs.receiveTime);
}

const Crypto::Hash& TransactionPool::PendingTransactionInfo::getTransactionHash() const {
  return cachedTransaction.getTransactionHash();
}

size_t TransactionPool::PaymentIdHasher::operator() (const boost::optional<Crypto::Hash>& paymentId) const {
  if (!paymentId) {
    return std::numeric_limits<size_t>::max();
  }

  return std::hash<Crypto::Hash>{}(*paymentId);
}

TransactionPool::TransactionPool(Logging::ILogger& logger) :
  transactionHashIndex(transactions.get<TransactionHashTag>()),
  transactionCostIndex(transactions.get<TransactionCostTag>()),
  paymentIdIndex(transactions.get<PaymentIdTag>()),
  logger(logger, "TransactionPool") {
}

bool TransactionPool::pushTransaction(CachedTransaction&& transaction, TransactionValidatorState&& transactionState) {
  auto pendingTx = PendingTransactionInfo{static_cast<uint64_t>(time(nullptr)), std::move(transaction)};

  Crypto::Hash paymentId;
  if(getPaymentIdFromTxExtra(pendingTx.cachedTransaction.getTransaction().extra, paymentId)) {
    pendingTx.paymentId = paymentId;
  }

  if (transactionHashIndex.count(pendingTx.getTransactionHash()) > 0) {
    logger(Logging::DEBUGGING) << "pushTransaction: transaction hash already present in index";
    return false;
  }

  if (hasIntersections(poolState, transactionState)) {
    logger(Logging::DEBUGGING) << "pushTransaction: failed to merge states, some keys already used";
    return false;
  }

  mergeStates(poolState, transactionState);

  logger(Logging::DEBUGGING) << "pushed transaction " << pendingTx.getTransactionHash() << " to pool";
  return transactionHashIndex.emplace(std::move(pendingTx)).second;
}

const CachedTransaction& TransactionPool::getTransaction(const Crypto::Hash& hash) const {
  auto it = transactionHashIndex.find(hash);
  assert(it != transactionHashIndex.end());

  return it->cachedTransaction;
}

bool TransactionPool::removeTransaction(const Crypto::Hash& hash) {
  auto it = transactionHashIndex.find(hash);
  if (it == transactionHashIndex.end()) {
    logger(Logging::DEBUGGING) << "removeTransaction: transaction not found";
    return false;
  }

  excludeFromState(poolState, it->cachedTransaction);
  transactionHashIndex.erase(it);

  logger(Logging::DEBUGGING) << "transaction " << hash << " removed from pool";
  return true;
}

size_t TransactionPool::getTransactionCount() const {
  return transactionHashIndex.size();
}

std::vector<Crypto::Hash> TransactionPool::getTransactionHashes() const {
  std::vector<Crypto::Hash> hashes;
  for (auto it = transactionCostIndex.begin(); it != transactionCostIndex.end(); ++it) {
    hashes.push_back(it->getTransactionHash());
  }

  return hashes;
}

bool TransactionPool::checkIfTransactionPresent(const Crypto::Hash& hash) const {
  return transactionHashIndex.find(hash) != transactionHashIndex.end();
}

const TransactionValidatorState& TransactionPool::getPoolTransactionValidationState() const {
  return poolState;
}

std::vector<CachedTransaction> TransactionPool::getPoolTransactions() const {
  std::vector<CachedTransaction> result;
  result.reserve(transactionCostIndex.size());

  for (const auto& transactionItem: transactionCostIndex) {
    result.emplace_back(transactionItem.cachedTransaction);
  }

  return result;
}

uint64_t TransactionPool::getTransactionReceiveTime(const Crypto::Hash& hash) const {
  auto it = transactionHashIndex.find(hash);
  assert(it != transactionHashIndex.end());

  return it->receiveTime;
}

std::vector<Crypto::Hash> TransactionPool::getTransactionHashesByPaymentId(const Crypto::Hash& paymentId) const {
  boost::optional<Crypto::Hash> p(paymentId);

  auto range = paymentIdIndex.equal_range(p);
  std::vector<Crypto::Hash> transactionHashes;
  transactionHashes.reserve(std::distance(range.first, range.second));
  for (auto it = range.first; it != range.second; ++it) {
    transactionHashes.push_back(it->getTransactionHash());
  }

  return transactionHashes;
}

}
