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

#include <gtest/gtest.h>
#include <System/ContextGroupTimeout.h>
#include <System/InterruptedException.h>

using namespace System;

class ContextGroupTimeoutTest : public testing::Test {
public:
  ContextGroupTimeoutTest() : contextGroup(dispatcher), timer(dispatcher) {
  }

  Dispatcher dispatcher;
  ContextGroup contextGroup;
  Timer timer;
};

TEST_F(ContextGroupTimeoutTest, timeoutHappens) {
  auto begin = std::chrono::high_resolution_clock::now();
  ContextGroupTimeout groupTimeout(dispatcher, contextGroup, std::chrono::milliseconds(100));
  contextGroup.spawn([&] {
    EXPECT_THROW(Timer(dispatcher).sleep(std::chrono::milliseconds(200)), InterruptedException);
  });
  contextGroup.wait();
  ASSERT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - begin).count(), 50);
  ASSERT_TRUE((std::chrono::high_resolution_clock::now() - begin) < std::chrono::milliseconds(150));
}

TEST_F(ContextGroupTimeoutTest, timeoutSkipped) {
  auto begin = std::chrono::high_resolution_clock::now();
  {
    ContextGroupTimeout op(dispatcher, contextGroup, std::chrono::milliseconds(200));
    contextGroup.spawn([&] {
      EXPECT_NO_THROW(Timer(dispatcher).sleep(std::chrono::milliseconds(100)));
    });
    contextGroup.wait();
  }
  ASSERT_TRUE((std::chrono::high_resolution_clock::now() - begin) > std::chrono::milliseconds(50));
  ASSERT_TRUE((std::chrono::high_resolution_clock::now() - begin) < std::chrono::milliseconds(150));
}

TEST_F(ContextGroupTimeoutTest, noOperation) {
  ContextGroupTimeout op(dispatcher, contextGroup, std::chrono::milliseconds(100));
}
