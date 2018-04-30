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

#include "MainChainStorage.h"

#include <boost/filesystem.hpp>

#include "CryptoNoteTools.h"

namespace CryptoNote {

const size_t STORAGE_CACHE_SIZE = 100;

MainChainStorage::MainChainStorage(const std::string& blocksFilename, const std::string& indexesFilename) {
  if (!storage.open(blocksFilename, indexesFilename, STORAGE_CACHE_SIZE)) {
    throw std::runtime_error("Failed to load main chain storage: " + blocksFilename);
  }
}

MainChainStorage::~MainChainStorage() {
  storage.close();
}

void MainChainStorage::pushBlock(const RawBlock& rawBlock) {
  storage.push_back(rawBlock);
}

void MainChainStorage::popBlock() {
  storage.pop_back();
}

RawBlock MainChainStorage::getBlockByIndex(uint32_t index) const {
  if (index >= storage.size()) {
    throw std::out_of_range("Block index " + std::to_string(index) + " is out of range. Blocks count: " + std::to_string(storage.size()));
  }

  return storage[index];
}

uint32_t MainChainStorage::getBlockCount() const {
  return static_cast<uint32_t>(storage.size());
}

void MainChainStorage::clear() {
  storage.clear();
}

std::unique_ptr<IMainChainStorage> createSwappedMainChainStorage(const std::string& dataDir, const Currency& currency) {
  boost::filesystem::path blocksFilename = boost::filesystem::path(dataDir) / currency.blocksFileName();
  boost::filesystem::path indexesFilename = boost::filesystem::path(dataDir) / currency.blockIndexesFileName();

  std::unique_ptr<IMainChainStorage> storage(new MainChainStorage(blocksFilename.string(), indexesFilename.string()));
  if (storage->getBlockCount() == 0) {
    RawBlock genesis;
    genesis.block = toBinaryArray(currency.genesisBlock());
    storage->pushBlock(genesis);
  }

  return storage;
}

}
