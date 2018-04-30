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

#include <CryptoNote.h>

namespace CryptoNote {

//TODO: rename this class since it's not persistent blockchain storage!
class BlockchainStorage {
public:

  class IBlockchainStorageInternal {
  public:
    virtual ~IBlockchainStorageInternal() { }

    virtual void pushBlock(RawBlock&& rawBlock) = 0;

    //Returns IBlockchainStorageInternal with elements from [splitIndex, blocks.size() - 1].
    //Original IBlockchainStorageInternal will contain elements from [0, splitIndex - 1].
    virtual std::unique_ptr<IBlockchainStorageInternal> splitStorage(uint32_t splitIndex) = 0;

    virtual RawBlock getBlockByIndex(uint32_t index) const = 0;
    virtual uint32_t getBlockCount() const = 0;
  };

  explicit BlockchainStorage(uint32_t reserveSize);
  explicit BlockchainStorage(const std::string& indexFileName, const std::string& dataFileName);
  virtual ~BlockchainStorage();

  virtual void pushBlock(RawBlock&& rawBlock);

  //Returns BlockchainStorage with elements from [splitIndex, blocks.size() - 1].
  //Original BlockchainStorage will contain elements from [0, splitIndex - 1].
  virtual std::unique_ptr<BlockchainStorage> splitStorage(uint32_t splitIndex);

  virtual RawBlock getBlockByIndex(uint32_t index) const;
  virtual uint32_t getBlockCount() const;

private:
  std::unique_ptr<IBlockchainStorageInternal> internalStorage;

  explicit BlockchainStorage(std::unique_ptr<IBlockchainStorageInternal> storage);
};

}
