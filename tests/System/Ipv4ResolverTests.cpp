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

#include <System/Dispatcher.h>
#include <System/ContextGroup.h>
#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include <System/Ipv4Resolver.h>
#include <gtest/gtest.h>

using namespace System;

class Ipv4ResolverTests : public testing::Test {
public:
  Ipv4ResolverTests() : contextGroup(dispatcher), resolver(dispatcher) {
  }

  Dispatcher dispatcher;
  ContextGroup contextGroup;
  Ipv4Resolver resolver;
};

TEST_F(Ipv4ResolverTests, start) {
  contextGroup.spawn([&] { 
    ASSERT_NO_THROW(Ipv4Resolver(dispatcher).resolve("localhost")); 
  });
  contextGroup.wait();
}

TEST_F(Ipv4ResolverTests, stop) {
  contextGroup.spawn([&] {
    contextGroup.interrupt();
    ASSERT_THROW(resolver.resolve("localhost"), InterruptedException);
  });
  contextGroup.wait();
}

TEST_F(Ipv4ResolverTests, interruptWhileResolving) {
  contextGroup.spawn([&] {
    ASSERT_THROW(resolver.resolve("localhost"), InterruptedException);
  });
  contextGroup.interrupt();
  contextGroup.wait();
}

TEST_F(Ipv4ResolverTests, reuseAfterInterrupt) {
  contextGroup.spawn([&] {
    ASSERT_THROW(resolver.resolve("localhost"), InterruptedException);
  });
  contextGroup.interrupt();
  contextGroup.wait();
  contextGroup.spawn([&] {
    ASSERT_NO_THROW(resolver.resolve("localhost"));
  });
  contextGroup.wait();
}

TEST_F(Ipv4ResolverTests, resolve) {
  ASSERT_EQ(Ipv4Address("0.0.0.0"), resolver.resolve("0.0.0.0"));
  ASSERT_EQ(Ipv4Address("1.2.3.4"), resolver.resolve("1.2.3.4"));
  ASSERT_EQ(Ipv4Address("127.0.0.1"), resolver.resolve("127.0.0.1"));
  ASSERT_EQ(Ipv4Address("254.253.252.251"), resolver.resolve("254.253.252.251"));
  ASSERT_EQ(Ipv4Address("255.255.255.255"), resolver.resolve("255.255.255.255"));
  ASSERT_EQ(Ipv4Address("127.0.0.1"), resolver.resolve("localhost"));
//ASSERT_EQ(Ipv4Address("93.184.216.34"), resolver.resolve("example.com"));
  ASSERT_THROW(resolver.resolve(".0.0.0.0"), std::runtime_error);
  ASSERT_THROW(resolver.resolve("0..0.0.0"), std::runtime_error);
//ASSERT_THROW(resolver.resolve("0.0.0"), std::runtime_error);
  ASSERT_THROW(resolver.resolve("0.0.0."), std::runtime_error);
//ASSERT_THROW(resolver.resolve("0.0.0.0."), std::runtime_error);
  ASSERT_THROW(resolver.resolve("0.0.0.0.0"), std::runtime_error);
//ASSERT_THROW(resolver.resolve("0.0.0.00"), std::runtime_error);
//ASSERT_THROW(resolver.resolve("0.0.0.01"), std::runtime_error);
  ASSERT_THROW(resolver.resolve("0.0.0.256"), std::runtime_error);
//ASSERT_THROW(resolver.resolve("00.0.0.0"), std::runtime_error);
//ASSERT_THROW(resolver.resolve("01.0.0.0"), std::runtime_error);
  ASSERT_THROW(resolver.resolve("256.0.0.0"), std::runtime_error);
  ASSERT_THROW(resolver.resolve("invalid"), std::runtime_error);
}
