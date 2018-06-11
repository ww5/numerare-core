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
#include <unordered_set>
#include <atomic>
#include "net/net_utils_base.h"
#include "copyable_atomic.h"

namespace cryptonote
{

  struct cryptonote_connection_context: public epee::net_utils::connection_context_base
  {
    cryptonote_connection_context(): m_state(state_before_handshake), m_remote_blockchain_height(0), m_last_response_height(0),
        m_last_request_time(boost::posix_time::microsec_clock::universal_time()), m_callback_request_count(0), m_last_known_hash(crypto::null_hash) {}

    enum state
    {
      state_before_handshake = 0, //default state
      state_synchronizing,
      state_standby,
      state_idle,
      state_normal
    };

    state m_state;
    std::list<crypto::hash> m_needed_objects;
    std::unordered_set<crypto::hash> m_requested_objects;
    uint64_t m_remote_blockchain_height;
    uint64_t m_last_response_height;
    boost::posix_time::ptime m_last_request_time;
    epee::copyable_atomic m_callback_request_count; //in debug purpose: problem with double callback rise
    crypto::hash m_last_known_hash;
    //size_t m_score;  TODO: add score calculations
  };

  inline std::string get_protocol_state_string(cryptonote_connection_context::state s)
  {
    switch (s)
    {
    case cryptonote_connection_context::state_before_handshake:
      return "state_before_handshake";
    case cryptonote_connection_context::state_synchronizing:
      return "state_synchronizing";
    case cryptonote_connection_context::state_standby:
      return "state_standby";
    case cryptonote_connection_context::state_idle:
      return "state_idle";
    case cryptonote_connection_context::state_normal:
      return "state_normal";
    default:
      return "unknown";
    }    
  }

}
