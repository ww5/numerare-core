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

#include "cryptonote_protocol/cryptonote_protocol_handler.h"
#include "p2p/net_node.h"
#include "daemon/protocol.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize
{

class t_p2p final
{
private:
  typedef cryptonote::t_cryptonote_protocol_handler<cryptonote::core> t_protocol_raw;
  typedef nodetool::node_server<t_protocol_raw> t_node_server;
public:
  static void init_options(boost::program_options::options_description & option_spec)
  {
    t_node_server::init_options(option_spec);
  }
private:
  t_node_server m_server;
public:
  t_p2p(
      boost::program_options::variables_map const & vm
    , t_protocol & protocol
    )
    : m_server{protocol.get()}
  {
    //initialize objects
    MGINFO("Initializing p2p server...");
    if (!m_server.init(vm))
    {
      throw std::runtime_error("Failed to initialize p2p server.");
    }
    MGINFO("p2p server initialized OK");
  }

  t_node_server & get()
  {
    return m_server;
  }

  void run()
  {
    MGINFO("Starting p2p net loop...");
    m_server.run();
    MGINFO("p2p net loop stopped");
  }

  void stop()
  {
    m_server.send_stop_signal();
  }

  ~t_p2p()
  {
    MGINFO("Deinitializing p2p...");
    try {
      m_server.deinit();
    } catch (...) {
      MERROR("Failed to deinitialize p2p...");
    }
  }
};

}
