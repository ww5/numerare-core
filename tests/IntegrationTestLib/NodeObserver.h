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

#include "INode.h"
#include <mutex>
#include <condition_variable>

namespace CryptoNote {


template <typename T>
class ObservableValue {
public:
  ObservableValue(const T defaultValue = 0) : 
    m_prev(defaultValue), m_value(defaultValue) {
  }

  void init(T value) {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_value = m_prev = value;
  }

  void set(T value) {
    std::lock_guard<std::mutex> lk(m_mutex);
    m_value = value;
    m_cv.notify_all();
  }

  T get() {
    std::lock_guard<std::mutex> lk(m_mutex);
    return m_value;
  }

  bool waitFor(std::chrono::milliseconds ms, T& value) {
    std::unique_lock<std::mutex> lk(m_mutex);

    if (m_cv.wait_for(lk, ms, [this] { return m_prev != m_value; })) {
      value = m_prev = m_value;
      return true;
    }

    return false;
  }

  T wait() {
    std::unique_lock<std::mutex> lk(m_mutex);

    m_cv.wait(lk, [this] { return m_prev != m_value; });
    m_prev = m_value;
    return m_value;
  }

private:

  std::mutex m_mutex;
  std::condition_variable m_cv;

  T m_prev;
  T m_value;
};

class NodeObserver: public INodeObserver {
public:

  NodeObserver(INode& node) : m_node(node) {
    m_knownHeight.init(node.getLastKnownBlockHeight());
    node.addObserver(this);
  }

  ~NodeObserver() {
    m_node.removeObserver(this);
  }

  virtual void lastKnownBlockHeightUpdated(uint32_t height) override {
    m_knownHeight.set(height);
  }

  virtual void localBlockchainUpdated(uint32_t height) override {
    m_localHeight.set(height);
  }

  virtual void peerCountUpdated(size_t count) override {
    m_peerCount.set(count);
  }

  bool waitLastKnownBlockHeightUpdated(std::chrono::milliseconds ms, uint32_t& value) {
    return m_knownHeight.waitFor(ms, value);
  }

  bool waitLocalBlockchainUpdated(std::chrono::milliseconds ms, uint32_t& value) {
    return m_localHeight.waitFor(ms, value);
  }

  uint32_t waitLastKnownBlockHeightUpdated() {
    return m_knownHeight.wait();
  }

  ObservableValue<uint32_t> m_knownHeight;
  ObservableValue<uint32_t> m_localHeight;
  ObservableValue<size_t> m_peerCount;

private:

  INode& m_node;
};


}
