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

#include "DataBaseMock.h"

using namespace CryptoNote;

DataBaseMock::~DataBaseMock() {

}

std::error_code DataBaseMock::write(IWriteBatch& batch) {
  auto append = batch.extractRawDataToInsert();
  for (auto pr : append) {
    baseState[pr.first] = pr.second;
  }
  auto remove = batch.extractRawKeysToRemove();
  for (auto key : remove) {
    baseState.erase(key);
  }
  return{};
}

std::error_code DataBaseMock::writeSync(IWriteBatch& batch) {
  return write(batch);
}

std::error_code DataBaseMock::read(IReadBatch& batch) {
  auto keys = batch.getRawKeys();
  std::vector<std::string> kvs;
  std::vector<bool> states;
  for (auto key : keys) {
    auto it = baseState.find(key);
    if (it != baseState.end()) {
      kvs.push_back(it->second);
      states.push_back(true);
    } else {
      kvs.push_back("");
      states.push_back(false);
    }
  }

  batch.submitRawResult(kvs, states);
  return{};
}

std::unordered_map<uint32_t, RawBlock> DataBaseMock::blocks() {
  BlockchainReadBatch req;
  for (int i = 0; i < 30; ++i) {
    req.requestRawBlock(i);
  }
  read(req);
  auto result = req.extractResult();
  return result.getRawBlocks();
}
