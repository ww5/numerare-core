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

#include <System/Context.h>
#include <System/Dispatcher.h>
#include <System/Event.h>
#include <System/InterruptedException.h>
#include <System/Timer.h>
#include <gtest/gtest.h>

using namespace System;

TEST(ContextTests, getReturnsResult) {
  Dispatcher dispatcher;
  Context<int> context(dispatcher, [&] { 
    return 2; 
  });

  ASSERT_EQ(2, context.get());
}

TEST(ContextTests, getRethrowsException) {
  Dispatcher dispatcher;
  Context<> context(dispatcher, [&] {
    throw std::string("Hi there!"); 
  });

  ASSERT_THROW(context.get(), std::string);
}

TEST(ContextTests, destructorIgnoresException) {
  Dispatcher dispatcher;
  ASSERT_NO_THROW(Context<>(dispatcher, [&] {
    throw std::string("Hi there!");
  }));
}

TEST(ContextTests, interruptIsInterrupting) {
  Dispatcher dispatcher;
  Context<> context(dispatcher, [&] {
    if (dispatcher.interrupted()) {
      throw InterruptedException();
    }
  });

  context.interrupt();
  ASSERT_THROW(context.get(), InterruptedException);
}

TEST(ContextTests, getChecksInterruption) {
  Dispatcher dispatcher;
  Event event(dispatcher);
  Context<int> context1(dispatcher, [&] {
    event.wait();
    if (dispatcher.interrupted()) {
      return 11;
    }

    return 10;
  });

  Context<int> context2(dispatcher, [&] {
    event.set();
    return context1.get();
  });

  context2.interrupt();
  ASSERT_EQ(11, context2.get());
}

TEST(ContextTests, getIsInterruptible) {
  Dispatcher dispatcher;
  Event event1(dispatcher);
  Event event2(dispatcher);
  Context<int> context1(dispatcher, [&] {
    event2.wait();
    if (dispatcher.interrupted()) {
      return 11;
    }

    return 10;
  });

  Context<int> context2(dispatcher, [&] {
    event1.set();
    return context1.get();
  });

  event1.wait();
  context2.interrupt();
  event2.set();
  ASSERT_EQ(11, context2.get());
}

TEST(ContextTests, destructorInterrupts) {
  Dispatcher dispatcher;
  bool interrupted = false;
  {
    Context<> context(dispatcher, [&] {
      if (dispatcher.interrupted()) {
        interrupted = true;
      }
    });
  }

  ASSERT_TRUE(interrupted);
}
