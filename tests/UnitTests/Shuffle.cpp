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

#include <string>

#include "gtest/gtest.h"

#include <unordered_set>
#include "crypto/crypto.h"
#include "Common/ShuffleGenerator.h"

class ShuffleTest : public ::testing::Test {
public:

  typedef ShuffleGenerator<size_t, std::default_random_engine> DefaultShuffleGenerator;
  typedef ShuffleGenerator<size_t, Crypto::random_engine<size_t>> CryptoShuffleGenerator;

  template <typename Gen>
  void checkUniqueness(Gen& gen, size_t count) {

    std::unordered_set<size_t> values;

    for (auto i = 0; i < count; ++i) {
      auto value = gen();
      bool inserted = values.insert(value).second;
      EXPECT_TRUE(inserted);
    }
  }

  template <typename Gen>
  void consume(Gen& gen, size_t count) {
    for (auto i = 0; i < count; ++i) {
      gen();
    }
  }

  template <typename ShuffleT>
  void checkEngine(size_t N, size_t consumeCount, bool check) {
    ShuffleT gen(N);
    check ? checkUniqueness(gen, consumeCount) : consume(gen, consumeCount);
  }

};


namespace {
const size_t ITERATIONS = 10000;
}

TEST_F(ShuffleTest, correctness) {
  checkEngine<DefaultShuffleGenerator>(ITERATIONS, ITERATIONS, true);
}

TEST_F(ShuffleTest, correctness_fractionalSize) {
  checkEngine<DefaultShuffleGenerator>(ITERATIONS, ITERATIONS, true);
  checkEngine<DefaultShuffleGenerator>(ITERATIONS, ITERATIONS/2, true);
  checkEngine<DefaultShuffleGenerator>(ITERATIONS, ITERATIONS/3, true);
}


TEST_F(ShuffleTest, cryptoGenerator) {
  checkEngine<CryptoShuffleGenerator>(ITERATIONS * 3, ITERATIONS, false);
}
