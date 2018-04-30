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

#include <boost/optional.hpp>
#include <CryptoNote.h>

namespace Crypto {

class cn_context;

}

namespace CryptoNote {

class CachedBlock {
public:
  explicit CachedBlock(const BlockTemplate& block);
  const BlockTemplate& getBlock() const;
  const Crypto::Hash& getTransactionTreeHash() const;
  const Crypto::Hash& getBlockHash() const;
  const Crypto::Hash& getBlockLongHash(Crypto::cn_context& cryptoContext) const;
  const Crypto::Hash& getAuxiliaryBlockHeaderHash() const;
  const BinaryArray& getBlockHashingBinaryArray() const;
  const BinaryArray& getParentBlockBinaryArray(bool headerOnly) const;
  const BinaryArray& getParentBlockHashingBinaryArray(bool headerOnly) const;
  uint32_t getBlockIndex() const;

private:
  const BlockTemplate& block;
  mutable boost::optional<BinaryArray> blockHashingBinaryArray;
  mutable boost::optional<BinaryArray> parentBlockBinaryArray;
  mutable boost::optional<BinaryArray> parentBlockHashingBinaryArray;
  mutable boost::optional<BinaryArray> parentBlockBinaryArrayHeaderOnly;
  mutable boost::optional<BinaryArray> parentBlockHashingBinaryArrayHeaderOnly;
  mutable boost::optional<uint32_t> blockIndex;
  mutable boost::optional<Crypto::Hash> transactionTreeHash;
  mutable boost::optional<Crypto::Hash> blockHash;
  mutable boost::optional<Crypto::Hash> blockLongHash;
  mutable boost::optional<Crypto::Hash> auxiliaryBlockHeaderHash;
};

}
