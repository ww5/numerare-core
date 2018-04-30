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

#include "BlockchainStorage.h"

#include "MemoryBlockchainStorage.h"
#include "SwappedBlockchainStorage.h"

using namespace CryptoNote;

BlockchainStorage::BlockchainStorage(uint32_t reserveSize) : 
internalStorage(new MemoryBlockchainStorage(reserveSize)) {
}

BlockchainStorage::BlockchainStorage(const std::string& indexFileName, const std::string& dataFileName) : 
internalStorage(new SwappedBlockchainStorage(indexFileName, dataFileName)) {
}

BlockchainStorage::BlockchainStorage(std::unique_ptr<IBlockchainStorageInternal> storage) :
internalStorage(std::move(storage)) {
}

BlockchainStorage::~BlockchainStorage() {
}

void BlockchainStorage::pushBlock(RawBlock&& rawBlock) {
  internalStorage->pushBlock(std::move(rawBlock));
}

RawBlock BlockchainStorage::getBlockByIndex(uint32_t index) const {
  return internalStorage->getBlockByIndex(index);
}

uint32_t BlockchainStorage::getBlockCount() const {
  return internalStorage->getBlockCount();
}

//Returns MemoryBlockchainStorage with elements from [splitIndex, blocks.size() - 1].
//Original MemoryBlockchainStorage will contain elements from [0, splitIndex - 1].
std::unique_ptr<BlockchainStorage> BlockchainStorage::splitStorage(uint32_t splitIndex) {
  std::unique_ptr<BlockchainStorage> newStorage(new BlockchainStorage(internalStorage->splitStorage(splitIndex)));
  return newStorage;
}
