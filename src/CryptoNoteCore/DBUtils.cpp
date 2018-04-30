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

#include "DBUtils.h"

namespace {
  const std::string RAW_BLOCK_NAME = "raw_block";
  const std::string RAW_TXS_NAME = "raw_txs";
}

namespace CryptoNote {
namespace DB {
  std::string serialize(const RawBlock& value, const std::string& name) {
    std::stringstream ss;
    Common::StdOutputStream stream(ss);
    CryptoNote::BinaryOutputStreamSerializer serializer(stream);
    
    serializer(const_cast<RawBlock&>(value).block, RAW_BLOCK_NAME);
    serializer(const_cast<RawBlock&>(value).transactions, RAW_TXS_NAME);

    return ss.str();
  }

  void deserialize(const std::string& serialized, RawBlock& value, const std::string& name) {
    std::stringstream ss(serialized);
    Common::StdInputStream stream(ss);
    CryptoNote::BinaryInputStreamSerializer serializer(stream);
    serializer(value.block, RAW_BLOCK_NAME);
    serializer(value.transactions, RAW_TXS_NAME);
  }
}
}
