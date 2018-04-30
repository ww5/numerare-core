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

#include <chrono>
#include <vector>

#include <System/ContextGroup.h>
#include <System/Dispatcher.h>
#include <System/Event.h>
#include <System/TcpConnection.h>
#include <System/Timer.h>

#include "CryptoNoteConfig.h"
#include "LevinProtocol.h"
#include "P2pInterfaces.h"
#include "P2pProtocolDefinitions.h"
#include "P2pProtocolTypes.h"

namespace CryptoNote {
  
class P2pContext {
public:
  using Clock = std::chrono::steady_clock;
  using TimePoint = Clock::time_point;

  struct Message : P2pMessage {
    enum Type {
      NOTIFY,
      REQUEST,
      REPLY
    };

    Type messageType;
    uint32_t returnCode;

    Message(P2pMessage&& msg, Type messageType, uint32_t returnCode = 0);
    size_t size() const;
  };

  P2pContext(System::Dispatcher& dispatcher, System::TcpConnection&& conn, 
    bool isIncoming, const NetworkAddress& remoteAddress, std::chrono::nanoseconds timedSyncInterval, const CORE_SYNC_DATA& timedSyncData);
  ~P2pContext();

  PeerIdType getPeerId() const;
  uint16_t getPeerPort() const;
  const NetworkAddress& getRemoteAddress() const;
  bool isIncoming() const;

  void setPeerInfo(uint8_t protocolVersion, PeerIdType id, uint16_t port);
  bool readCommand(LevinProtocol::Command& cmd);
  void writeMessage(const Message& msg);
 
  void start();
  void stop();

private:

  uint8_t version = 0;
  const bool incoming;
  const NetworkAddress remoteAddress;
  PeerIdType peerId = 0;
  uint16_t peerPort = 0;

  System::Dispatcher& dispatcher;
  System::ContextGroup contextGroup;
  const TimePoint timeStarted;
  bool stopped = false;
  TimePoint lastReadTime;

  // timed sync info
  const std::chrono::nanoseconds timedSyncInterval;
  const CORE_SYNC_DATA& timedSyncData;
  System::Timer timedSyncTimer;
  System::Event timedSyncFinished;

  System::TcpConnection connection;
  System::Event writeEvent;
  System::Event readEvent;

  void timedSyncLoop();
};

P2pContext::Message makeReply(uint32_t command, const BinaryArray& data, uint32_t returnCode);
P2pContext::Message makeRequest(uint32_t command, const BinaryArray& data);

std::ostream& operator <<(std::ostream& s, const P2pContext& conn);

}
