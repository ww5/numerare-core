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

#include "Common/StringTools.h"
#include "crypto/crypto.h"
#include "CryptoNoteCore/CryptoNoteBasic.h"

class test_cn_slow_hash {
public:
  static const size_t loop_count = 10;

#pragma pack(push, 1)
  struct data_t {
    char data[13];
  };
#pragma pack(pop)

  static_assert(13 == sizeof(data_t), "Invalid structure size");

  bool init() {
    size_t size;
    if (!Common::fromHex("63617665617420656d70746f72", &m_data, sizeof(m_data), size) || size != sizeof(m_data)) {
      return false;
    }

    if (!Common::fromHex("bbec2cacf69866a8e740380fe7b818fc78f8571221742d729d9d02d7f8989b87", &m_expected_hash, sizeof(m_expected_hash), size) || size != sizeof(m_expected_hash)) {
      return false;
    }

    return true;
  }

  bool test() {
    Crypto::Hash hash;
    Crypto::cn_slow_hash(m_context, &m_data, sizeof(m_data), hash);
    return hash == m_expected_hash;
  }

private:
  data_t m_data;
  Crypto::Hash m_expected_hash;
  Crypto::cn_context m_context;
};
