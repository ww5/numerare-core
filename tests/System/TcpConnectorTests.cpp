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
#include <System/Event.h>
#include <System/InterruptedException.h>
#include <System/Ipv4Address.h>
#include <System/TcpConnection.h>
#include <System/TcpConnector.h>
#include <System/TcpListener.h>
#include <System/Timer.h>
#include <gtest/gtest.h>

using namespace System;

class TcpConnectorTests : public testing::Test {
public:
  TcpConnectorTests() : event(dispatcher), listener(dispatcher, Ipv4Address("127.0.0.1"), 6666), contextGroup(dispatcher) {
  }
  
  Dispatcher dispatcher;
  Event event;
  TcpListener listener;
  ContextGroup contextGroup;
};

TEST_F(TcpConnectorTests, tcpConnector1) {
  contextGroup.spawn([&]() {
    listener.accept();
    event.set();
  });

  TcpConnector connector(dispatcher);
  contextGroup.spawn([&] { 
    connector.connect(Ipv4Address("127.0.0.1"), 6666); 
  });
  event.wait();
  dispatcher.yield();
}

TEST_F(TcpConnectorTests, tcpConnectorInterruptAfterStart) {
  contextGroup.spawn([&] { 
    ASSERT_THROW(TcpConnector(dispatcher).connect(Ipv4Address("127.0.0.1"), 6666), InterruptedException); 
  });
  contextGroup.interrupt();
}

TEST_F(TcpConnectorTests, tcpConnectorInterrupt) {
  TcpConnector connector(dispatcher);
  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
    event.set();
  });

  contextGroup.spawn([&] { 
    ASSERT_THROW(connector.connect(Ipv4Address("10.255.255.1"), 6666), InterruptedException); 
  });
  contextGroup.wait();
}

TEST_F(TcpConnectorTests, tcpConnectorUseAfterInterrupt) {
  TcpConnector connector(dispatcher);
  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
    event.set();
  });

  contextGroup.spawn([&] { 
    ASSERT_THROW(connector.connect(Ipv4Address("10.255.255.1"), 6666), InterruptedException); 
  });
  contextGroup.wait();
  contextGroup.spawn([&] { 
    ASSERT_NO_THROW(connector.connect(Ipv4Address("127.0.0.1"), 6666)); 
  });
  contextGroup.wait();
}

TEST_F(TcpConnectorTests, bindToTheSameAddressFails) {
  ASSERT_THROW(TcpListener listener2(dispatcher, Ipv4Address("127.0.0.1"), 6666), std::runtime_error);
}
