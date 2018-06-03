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

	Parts of this file are originally Copyright (c) 2012-2014 The CryptoNote developers,
	                                                2014-2018 The Monero Project
***/

#pragma once

#include <string>
#include <boost/thread/mutex.hpp>
#include "include_base_utils.h"
#include "net/http_client.h"

namespace tools
{

class NodeRPCProxy
{
public:
  NodeRPCProxy(epee::net_utils::http::http_simple_client &http_client, boost::mutex &mutex);

  void invalidate();

  boost::optional<std::string> get_rpc_version(uint32_t &version) const;
  boost::optional<std::string> get_height(uint64_t &height) const;
  void set_height(uint64_t h);
  boost::optional<std::string> get_target_height(uint64_t &height) const;
  boost::optional<std::string> get_earliest_height(uint8_t version, uint64_t &earliest_height) const;
  boost::optional<std::string> get_dynamic_per_kb_fee_estimate(uint64_t grace_blocks, uint64_t &fee) const;

private:
  epee::net_utils::http::http_simple_client &m_http_client;
  boost::mutex &m_daemon_rpc_mutex;

  mutable uint64_t m_height;
  mutable time_t m_height_time;
  mutable uint64_t m_earliest_height[256];
  mutable uint64_t m_dynamic_per_kb_fee_estimate;
  mutable uint64_t m_dynamic_per_kb_fee_estimate_cached_height;
  mutable uint64_t m_dynamic_per_kb_fee_estimate_grace_blocks;
  mutable uint32_t m_rpc_version;
  mutable uint64_t m_target_height;
  mutable time_t m_target_height_time;
};

}
