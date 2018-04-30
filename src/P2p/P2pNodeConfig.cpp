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

#include "P2pNodeConfig.h"
#include "P2pNetworks.h"

#include <CryptoNoteConfig.h>

namespace CryptoNote {

namespace {

const std::chrono::nanoseconds P2P_DEFAULT_CONNECT_INTERVAL = std::chrono::seconds(2);
const size_t P2P_DEFAULT_CONNECT_RANGE = 20;
const size_t P2P_DEFAULT_PEERLIST_GET_TRY_COUNT = 10;

}

P2pNodeConfig::P2pNodeConfig() :
  timedSyncInterval(std::chrono::seconds(P2P_DEFAULT_HANDSHAKE_INTERVAL)),
  handshakeTimeout(std::chrono::milliseconds(P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT)),
  connectInterval(P2P_DEFAULT_CONNECT_INTERVAL),
  connectTimeout(std::chrono::milliseconds(P2P_DEFAULT_CONNECTION_TIMEOUT)),
  networkId(BYTECOIN_NETWORK),
  expectedOutgoingConnectionsCount(P2P_DEFAULT_CONNECTIONS_COUNT),
  whiteListConnectionsPercent(P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT),
  peerListConnectRange(P2P_DEFAULT_CONNECT_RANGE),
  peerListGetTryCount(P2P_DEFAULT_PEERLIST_GET_TRY_COUNT) {
}

// getters

std::chrono::nanoseconds P2pNodeConfig::getTimedSyncInterval() const {
  return timedSyncInterval;
}

std::chrono::nanoseconds P2pNodeConfig::getHandshakeTimeout() const {
  return handshakeTimeout;
}

std::chrono::nanoseconds P2pNodeConfig::getConnectInterval() const {
  return connectInterval;
}

std::chrono::nanoseconds P2pNodeConfig::getConnectTimeout() const {
  return connectTimeout;
}

size_t P2pNodeConfig::getExpectedOutgoingConnectionsCount() const {
  return expectedOutgoingConnectionsCount;
}

size_t P2pNodeConfig::getWhiteListConnectionsPercent() const {
  return whiteListConnectionsPercent;
}

boost::uuids::uuid P2pNodeConfig::getNetworkId() const {
  if (getTestnet()) {
    boost::uuids::uuid copy = networkId;
    copy.data[0] += 1;
    return copy;
  }
  return networkId;
}

size_t P2pNodeConfig::getPeerListConnectRange() const {
  return peerListConnectRange;
}

size_t P2pNodeConfig::getPeerListGetTryCount() const {
  return peerListGetTryCount;
}

// setters

void P2pNodeConfig::setTimedSyncInterval(std::chrono::nanoseconds interval) {
  timedSyncInterval = interval;
}

void P2pNodeConfig::setHandshakeTimeout(std::chrono::nanoseconds timeout) {
  handshakeTimeout = timeout;
}

void P2pNodeConfig::setConnectInterval(std::chrono::nanoseconds interval) {
  connectInterval = interval;
}

void P2pNodeConfig::setConnectTimeout(std::chrono::nanoseconds timeout) {
  connectTimeout = timeout;
}

void P2pNodeConfig::setExpectedOutgoingConnectionsCount(size_t count) {
  expectedOutgoingConnectionsCount = count;
}

void P2pNodeConfig::setWhiteListConnectionsPercent(size_t percent) {
  if (percent > 100) {
    throw std::invalid_argument("whiteListConnectionsPercent cannot be greater than 100");
  }

  whiteListConnectionsPercent = percent;
}

void P2pNodeConfig::setNetworkId(const boost::uuids::uuid& id) {
  networkId = id;
}

void P2pNodeConfig::setPeerListConnectRange(size_t range) {
  peerListConnectRange = range;
}

void P2pNodeConfig::setPeerListGetTryCount(size_t count) {
  peerListGetTryCount = count;
}

}
