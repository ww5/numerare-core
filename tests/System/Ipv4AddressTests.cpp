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

#include <System/Ipv4Address.h>
#include <gtest/gtest.h>

using namespace System;

TEST(Ipv4AddressTest, value) {
  Ipv4Address address1(0x00000000);
  ASSERT_EQ(0x00000000, address1.getValue());
  Ipv4Address address2(0xfefdfcfb);
  ASSERT_EQ(0xfefdfcfb, address2.getValue());
  Ipv4Address address3 = address1;
  ASSERT_EQ(0x00000000, address3.getValue());
  Ipv4Address address4 = address2;
  ASSERT_EQ(0xfefdfcfb, address4.getValue());
  address3 = address2;
  ASSERT_EQ(0xfefdfcfb, address3.getValue());
  address4 = address1;
  ASSERT_EQ(0x00000000, address4.getValue());
}

TEST(Ipv4AddressTest, dottedDecimal) {
  ASSERT_EQ(0x00000000, Ipv4Address("0.0.0.0").getValue());
  ASSERT_EQ(0x01020304, Ipv4Address("1.2.3.4").getValue());
  ASSERT_EQ(0x7f000001, Ipv4Address("127.0.0.1").getValue());
  ASSERT_EQ(0xfefdfcfb, Ipv4Address("254.253.252.251").getValue());
  ASSERT_EQ(0xffffffff, Ipv4Address("255.255.255.255").getValue());
  ASSERT_EQ("0.0.0.0", Ipv4Address(0x00000000).toDottedDecimal());
  ASSERT_EQ("1.2.3.4", Ipv4Address(0x01020304).toDottedDecimal());
  ASSERT_EQ("127.0.0.1", Ipv4Address(0x7f000001).toDottedDecimal());
  ASSERT_EQ("254.253.252.251", Ipv4Address(0xfefdfcfb).toDottedDecimal());
  ASSERT_EQ("255.255.255.255", Ipv4Address(0xffffffff).toDottedDecimal());
  ASSERT_THROW(Ipv4Address(".0.0.0.0"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("0..0.0.0"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("0.0.0"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("0.0.0."), std::runtime_error);
  ASSERT_THROW(Ipv4Address("0.0.0.0."), std::runtime_error);
  ASSERT_THROW(Ipv4Address("0.0.0.0.0"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("0.0.0.00"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("0.0.0.01"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("0.0.0.256"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("00.0.0.0"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("01.0.0.0"), std::runtime_error);
  ASSERT_THROW(Ipv4Address("256.0.0.0"), std::runtime_error);
}

TEST(Ipv4AddressTest, isLoopback) {
  // 127.0.0.0/8
  ASSERT_TRUE(Ipv4Address("127.0.0.1").isLoopback());
  ASSERT_TRUE(Ipv4Address("127.1.1.1").isLoopback());
  ASSERT_TRUE(Ipv4Address("127.1.0.0").isLoopback());
  ASSERT_TRUE(Ipv4Address("127.255.255.255").isLoopback());

  ASSERT_FALSE(Ipv4Address("255.0.0.0").isLoopback());
  ASSERT_FALSE(Ipv4Address("255.255.255.255").isLoopback());
  ASSERT_FALSE(Ipv4Address("128.1.0.0").isLoopback());
  ASSERT_FALSE(Ipv4Address("192.168.1.1").isLoopback());
  ASSERT_FALSE(Ipv4Address("10.0.0.1").isLoopback());
}

TEST(Ipv4AddressTest, isPrivate) {
  // 10.0.0.0/8
  ASSERT_TRUE(Ipv4Address("10.0.0.0").isPrivate());
  ASSERT_TRUE(Ipv4Address("10.0.0.1").isPrivate());
  ASSERT_TRUE(Ipv4Address("10.0.0.255").isPrivate());
  ASSERT_TRUE(Ipv4Address("10.255.255.255").isPrivate());

  ASSERT_FALSE(Ipv4Address("11.0.0.255").isPrivate());
  ASSERT_FALSE(Ipv4Address("9.0.0.0").isPrivate());
  ASSERT_FALSE(Ipv4Address("138.0.0.1").isPrivate());
  ASSERT_FALSE(Ipv4Address("255.255.255.255").isPrivate());

  // 172.16.0.0/12 
  ASSERT_TRUE(Ipv4Address("172.16.0.255").isPrivate());
  ASSERT_TRUE(Ipv4Address("172.17.0.0").isPrivate());
  ASSERT_TRUE(Ipv4Address("172.19.1.1").isPrivate());
  ASSERT_TRUE(Ipv4Address("172.31.255.255").isPrivate());

  ASSERT_FALSE(Ipv4Address("172.32.0.0").isPrivate());
  ASSERT_FALSE(Ipv4Address("172.32.0.1").isPrivate());
  ASSERT_FALSE(Ipv4Address("172.15.0.0").isPrivate());
  ASSERT_FALSE(Ipv4Address("172.15.255.255").isPrivate());

  // 192.168.0.0/16
  ASSERT_TRUE(Ipv4Address("192.168.0.0").isPrivate());
  ASSERT_TRUE(Ipv4Address("192.168.1.1").isPrivate());
  ASSERT_TRUE(Ipv4Address("192.168.100.100").isPrivate());
  ASSERT_TRUE(Ipv4Address("192.168.255.255").isPrivate());

  ASSERT_FALSE(Ipv4Address("192.167.255.255").isPrivate());
  ASSERT_FALSE(Ipv4Address("191.168.255.255").isPrivate());
  ASSERT_FALSE(Ipv4Address("192.169.255.255").isPrivate());
  ASSERT_FALSE(Ipv4Address("192.169.0.0").isPrivate());

  ASSERT_FALSE(Ipv4Address("255.255.255.255").isPrivate());

}
