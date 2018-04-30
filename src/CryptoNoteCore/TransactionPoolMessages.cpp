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

#include <CryptoNoteCore/TransactionPoolMessages.h>

namespace CryptoNote {

TransactionPoolMessage::TransactionPoolMessage(const AddTransaction& at)
    : type(TransactionMessageType::AddTransactionType), addTransaction(at) {
}

TransactionPoolMessage::TransactionPoolMessage(const DeleteTransaction& dt)
    : type(TransactionMessageType::DeleteTransactionType), deleteTransaction(dt) {
}

// pattern match
void TransactionPoolMessage::match(std::function<void(const AddTransaction&)>&& addTxVisitor,
                                   std::function<void(const DeleteTransaction&)>&& delTxVisitor) {
  switch (getType()) {
    case TransactionMessageType::AddTransactionType:
      addTxVisitor(addTransaction);
      break;
    case TransactionMessageType::DeleteTransactionType:
      delTxVisitor(deleteTransaction);
      break;
  }
}

// API with explicit type handling
TransactionMessageType TransactionPoolMessage::getType() const {
  return type;
}

AddTransaction TransactionPoolMessage::getAddTransaction() const {
  assert(getType() == TransactionMessageType::AddTransactionType);
  return addTransaction;
}

DeleteTransaction TransactionPoolMessage::getDeleteTransaction() const {
  assert(getType() == TransactionMessageType::DeleteTransactionType);
  return deleteTransaction;
}

TransactionPoolMessage makeAddTransaction(const Crypto::Hash& hash) {
  return TransactionPoolMessage{AddTransaction{hash}};
}

TransactionPoolMessage makeDelTransaction(const Crypto::Hash& hash) {
  return TransactionPoolMessage{DeleteTransaction{hash}};
}
}
