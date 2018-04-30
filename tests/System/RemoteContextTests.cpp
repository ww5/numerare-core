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

#include <System/RemoteContext.h>
#include <System/Dispatcher.h>
#include <System/ContextGroup.h>
#include <System/Event.h>
#include <System/InterruptedException.h>
#include <System/Timer.h>
#include <gtest/gtest.h>

using namespace System;

class RemoteContextTests : public testing::Test {
public:
  Dispatcher dispatcher;
};


TEST_F(RemoteContextTests, getReturnsResult) {
  RemoteContext<int> context(dispatcher, [&] { 
    return 2; 
  });

  ASSERT_EQ(2, context.get());
}

TEST_F(RemoteContextTests, getRethrowsException) {
  RemoteContext<> context(dispatcher, [&] {
    throw std::string("Hi there!"); 
  });

  ASSERT_THROW(context.get(), std::string);
}

TEST_F(RemoteContextTests, destructorIgnoresException) {
  ASSERT_NO_THROW(RemoteContext<>(dispatcher, [&] {
    throw std::string("Hi there!");
  }));
}

TEST_F(RemoteContextTests, canBeUsedWithoutObject) {
  ASSERT_EQ(42, RemoteContext<int>(dispatcher, [&] { return 42; }).get());
}

TEST_F(RemoteContextTests, interruptIsInterruptingWait) {
  ContextGroup cg(dispatcher);
  bool started = false;

  cg.spawn([&] {
    RemoteContext<> context(dispatcher, [&] {
      started = true;
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });
    ASSERT_NO_THROW(context.wait());
    ASSERT_TRUE(dispatcher.interrupted());
  });

  cg.interrupt();
  cg.wait();

  ASSERT_TRUE(started);
}

TEST_F(RemoteContextTests, interruptIsInterruptingGet) {
  ContextGroup cg(dispatcher);
  cg.spawn([&] {
    RemoteContext<> context(dispatcher, [&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });
    ASSERT_NO_THROW(context.get());
    ASSERT_TRUE(dispatcher.interrupted());
  });

  cg.interrupt();
  cg.wait();
}

TEST_F(RemoteContextTests, destructorIgnoresInterrupt) {
  ContextGroup cg(dispatcher);
  cg.spawn([&] {
    ASSERT_NO_THROW(RemoteContext<>(dispatcher, [&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }));
  });

  cg.interrupt();
  cg.wait();
}

TEST_F(RemoteContextTests, canExecuteOtherContextsWhileWaiting) {
  auto start = std::chrono::high_resolution_clock::now();
  ContextGroup cg(dispatcher);
  cg.spawn([&] {
    RemoteContext<> context(dispatcher, [&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });
  });
  cg.spawn([&] {
    System::Timer(dispatcher).sleep(std::chrono::milliseconds(50));
    auto end = std::chrono::high_resolution_clock::now();
    ASSERT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), 50);
    ASSERT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), 100);
  });

  cg.wait();
}

TEST_F(RemoteContextTests, waitMethodWaitsForContexCompletion) {
  auto start = std::chrono::high_resolution_clock::now();
  ContextGroup cg(dispatcher);
  cg.spawn([&] {
    RemoteContext<> context(dispatcher, [&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });
  });

  cg.wait();
  auto end = std::chrono::high_resolution_clock::now();
  ASSERT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), 10);
}

TEST_F(RemoteContextTests, waitMethodWaitsForContexCompletionOnInterrupt) {
  auto start = std::chrono::high_resolution_clock::now();
  ContextGroup cg(dispatcher);
  cg.spawn([&] {
    RemoteContext<> context(dispatcher, [&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });
  });

  cg.interrupt();
  cg.wait();
  auto end = std::chrono::high_resolution_clock::now();
  ASSERT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), 10);
}

