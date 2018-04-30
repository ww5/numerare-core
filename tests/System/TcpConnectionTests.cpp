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
#include <System/TcpStream.h>
#include <System/Timer.h>
#include <gtest/gtest.h>

using namespace System;

namespace {

const Ipv4Address LISTEN_ADDRESS("127.0.0.1");
const uint16_t LISTEN_PORT = 6666;

void fillRandomBuf(std::vector<uint8_t>& buf) {
  for (size_t i = 0; i < buf.size(); ++i) {
    buf[i] = static_cast<uint8_t>(rand() & 0xff);
  }
}

void fillRandomString(std::string& buf) {
  for (size_t i = 0; i < buf.size(); ++i) {
    buf[i] = static_cast<uint8_t>(rand() & 0xff);
  }
}

std::string removePort(const std::string& address) {
  size_t colonPosition = address.rfind(':');
  if (colonPosition == std::string::npos) {
    throw std::runtime_error("removePort");
  }

  return address.substr(0, colonPosition);
}

}

class TcpConnectionTests : public testing::Test {
public:
  TcpConnectionTests() : listener(dispatcher, LISTEN_ADDRESS, LISTEN_PORT), contextGroup(dispatcher) {
  }

  void connect() {
    connection1 = TcpConnector(dispatcher).connect(LISTEN_ADDRESS, LISTEN_PORT);
    connection2 = listener.accept();
  }

protected:
  Dispatcher dispatcher;
  TcpListener listener;
  TcpConnection connection1;
  TcpConnection connection2;
  ContextGroup contextGroup;
};

TEST_F(TcpConnectionTests, sendAndClose) {
  connect();
  ASSERT_EQ(LISTEN_ADDRESS, connection1.getPeerAddressAndPort().first);
  ASSERT_EQ(LISTEN_ADDRESS, connection2.getPeerAddressAndPort().first);
  connection1.write(reinterpret_cast<const uint8_t*>("Test"), 4);
  uint8_t data[1024];
  size_t size = connection2.read(data, 1024);
  ASSERT_EQ(4, size);
  ASSERT_EQ(0, memcmp(data, "Test", 4));
  connection1 = TcpConnection();
  size = connection2.read(data, 1024);
  ASSERT_EQ(0, size);
}

TEST_F(TcpConnectionTests, stoppedState) {
  connect();
  bool stopped = false;
  contextGroup.spawn([&] {
    try {
      uint8_t data[1024];
      connection1.read(data, 1024);
    } catch (InterruptedException&) {
      stopped = true;
    }

    ASSERT_TRUE(stopped);
    contextGroup.interrupt();
    stopped = false;
    try {
      connection1.write(reinterpret_cast<const uint8_t*>("Test"), 4);
    } catch (InterruptedException&) {
      stopped = true;
    }
  });
  contextGroup.interrupt();
  contextGroup.wait();

  ASSERT_TRUE(stopped);
}

TEST_F(TcpConnectionTests, interruptRead) {
  connect();
  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
  });

  bool stopped = false;
  contextGroup.spawn([&]() {
    try {
      uint8_t data[1024];
      connection1.read(data, 1024);
    } catch (InterruptedException &) {
      stopped = true;
    }
  });

  contextGroup.wait();
  ASSERT_TRUE(stopped);
}

TEST_F(TcpConnectionTests, reuseWriteAfterInterrupt) {
  connect();
  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
  });

  bool stopped = false;
  contextGroup.spawn([&]() {
    try {
      uint8_t data[1024];
      connection1.read(data, 1024);
    } catch (InterruptedException &) {
      stopped = true;
    }
  });

  contextGroup.wait();
  ASSERT_TRUE(stopped);
  stopped = false;

  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
  });

  contextGroup.spawn([&] {
    try {
      uint8_t buff[1024];
      std::fill(std::begin(buff), std::end(buff), 0xff);
      connection1.write(buff, sizeof(buff)); // write smth
      connection1 = TcpConnection();         // close connection
    } catch (InterruptedException&) {
      stopped = true;
    }
  });

  contextGroup.spawn([&]() {
    try {
      uint8_t data[1024];
      connection2.read(data, 1024);
    } catch (InterruptedException &) {
      stopped = true;
    }
  });

  contextGroup.wait();
  ASSERT_TRUE(!stopped);
}

TEST_F(TcpConnectionTests, reuseReadAfterInterrupt) {
  connect();
  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
  });

  bool stopped = false;
  contextGroup.spawn([&]() {
    try {
      uint8_t data[1024];
      connection1.read(data, 1024);
    } catch (InterruptedException &) {
      stopped = true;
    }
  });

  contextGroup.wait();
  ASSERT_TRUE(stopped);
  stopped = false;

  contextGroup.spawn([&]() {
    Timer(dispatcher).sleep(std::chrono::milliseconds(10));
    contextGroup.interrupt();
  });

  contextGroup.spawn([&] {
    try {
      uint8_t buff[1024];
      std::fill(std::begin(buff), std::end(buff), 0xff);
      connection2.write(buff, sizeof(buff)); // write smth
      connection2 = TcpConnection();         // close connection
    } catch (InterruptedException&) {
      stopped = true;
    }
  });

  contextGroup.spawn([&]() {
    try {
      uint8_t data[1024];
      connection1.read(data, 1024);
    } catch (InterruptedException &) {
      stopped = true;
    }
  });

  contextGroup.wait();
  ASSERT_TRUE(!stopped);
}

TEST_F(TcpConnectionTests, sendBigChunk) {
  connect();
  
  const size_t bufsize =  15* 1024 * 1024; // 15MB
  std::vector<uint8_t> buf;
  buf.resize(bufsize);
  fillRandomBuf(buf);

  std::vector<uint8_t> incoming;
  Event readComplete(dispatcher);

  contextGroup.spawn([&]{
    uint8_t readBuf[1024];
    size_t readSize;
    while ((readSize = connection2.read(readBuf, sizeof(readBuf))) > 0) {
      incoming.insert(incoming.end(), readBuf, readBuf + readSize);
    }

    readComplete.set();
  });

  contextGroup.spawn([&]{
    uint8_t* bufPtr = &buf[0];
    size_t left = bufsize;
    while(left > 0) {
      auto transferred =  connection1.write(bufPtr, std::min(left, size_t(666)));
      left -= transferred;
      bufPtr += transferred;
    }

    connection1 = TcpConnection(); // close connection
  });

  readComplete.wait();

  ASSERT_EQ(bufsize, incoming.size());
  ASSERT_EQ(buf, incoming);
}

TEST_F(TcpConnectionTests, writeWhenReadWaiting) {
  connect();

  Event readStarted(dispatcher);
  Event readCompleted(dispatcher);
  Event writeCompleted(dispatcher);

  size_t writeSize = 0;
  bool readStopped = false;

  contextGroup.spawn([&]{
    try {
      uint8_t readBuf[1024];
      size_t readSize;
      readStarted.set();
      while ((readSize = connection2.read(readBuf, sizeof(readBuf))) > 0) {
      }
    } catch (InterruptedException&) {
      readStopped = true;
    }
    connection2 = TcpConnection();
    readCompleted.set();
  });

  readStarted.wait();

  contextGroup.spawn([&]{
    uint8_t writeBuf[1024];
    for (int i = 0; i < 100; ++i) {
      writeSize += connection2.write(writeBuf, sizeof(writeBuf));
    }
    //connection2.stop();
    contextGroup.interrupt();
    writeCompleted.set();
  });

  uint8_t readBuf[100];
  size_t readSize;
  size_t totalRead = 0;
  while ((readSize = connection1.read(readBuf, sizeof(readBuf))) > 0) {
    totalRead += readSize;
  }

  ASSERT_EQ(writeSize, totalRead);
  readCompleted.wait();
  ASSERT_TRUE(readStopped);
  writeCompleted.wait();
}

TEST_F(TcpConnectionTests, sendBigChunkThruTcpStream) {
  connect();
  const size_t bufsize = 15 * 1024 * 1024; // 15MB
  std::string buf;
  buf.resize(bufsize);
  fillRandomString(buf);

  std::string incoming;
  Event readComplete(dispatcher);

  contextGroup.spawn([&]{
    uint8_t readBuf[1024];
    size_t readSize;
    while ((readSize = connection2.read(readBuf, sizeof(readBuf))) > 0) {
      incoming.insert(incoming.end(), readBuf, readBuf + readSize);
    }

    readComplete.set();
  });


  contextGroup.spawn([&]{
    TcpStreambuf streambuf(connection1);
    std::iostream stream(&streambuf);

    stream << buf;
    stream.flush();

    connection1 = TcpConnection(); // close connection
  });

  readComplete.wait();

  ASSERT_EQ(bufsize, incoming.size());

  //ASSERT_EQ(buf, incoming); 
  for (size_t i = 0; i < bufsize; ++i) {
    ASSERT_EQ(buf[i], incoming[i]); //for better output.
  }
}
