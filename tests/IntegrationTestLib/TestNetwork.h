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

#include "NetworkConfiguration.h"
#include "Process.h"
#include "TestNode.h"

namespace System {
class Dispatcher;
}

namespace CryptoNote {
class Currency;
}

namespace Tests {

enum class Topology {
  Ring,
  Line,
  Star
};


class TestNetworkBuilder {
public:

  TestNetworkBuilder(size_t nodeCount, Topology topology = Topology::Line, uint16_t rpcBasePort = 9200, uint16_t p2pBasePort = 9000);

  TestNetworkBuilder& setDataDirectory(const std::string& dataDir);
  TestNetworkBuilder& setBlockchain(const std::string& blockchainDir);
  TestNetworkBuilder& setTestnet(bool isTestnet);

  std::vector<TestNodeConfiguration> build();

private:

  TestNodeConfiguration buildNodeConfiguration(size_t index);

  uint16_t rpcBasePort;
  uint16_t p2pBasePort;
  Topology topology;
  size_t nodeCount;
  std::string baseDataDir;
  std::string blockchainLocation;
  bool testnet;
};


class TestNetwork {

public:

  TestNetwork(System::Dispatcher& dispatcher, const CryptoNote::Currency& currency);

  void addNodes(const std::vector<TestNodeConfiguration>& nodes);
  void addNode(const TestNodeConfiguration& cfg);
  void waitNodesReady();
  void shutdown();

  TestNode& getNode(size_t index);

private:

  std::unique_ptr<TestNode> startDaemon(const TestNodeConfiguration& cfg);

  std::vector<std::pair<std::unique_ptr<TestNode>, TestNodeConfiguration>> nodes;
  System::Dispatcher& m_dispatcher;
  const CryptoNote::Currency& m_currency;
  std::vector<Process> m_daemons;
};

}
