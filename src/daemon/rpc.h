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

#include "rpc/core_rpc_server.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize
{

class t_rpc final
{
public:
  static void init_options(boost::program_options::options_description & option_spec)
  {
    cryptonote::core_rpc_server::init_options(option_spec);
  }
private:
  cryptonote::core_rpc_server m_server;
  const std::string m_description;
public:
  t_rpc(
      boost::program_options::variables_map const & vm
    , t_core & core
    , t_p2p & p2p
    , const bool restricted
    , const cryptonote::network_type nettype
    , const std::string & port
    , const std::string & description
    )
    : m_server{core.get(), p2p.get()}, m_description{description}
  {
    MGINFO("Initializing " << m_description << " RPC server...");

    if (!m_server.init(vm, restricted, nettype, port))
    {
      throw std::runtime_error("Failed to initialize " + m_description + " RPC server.");
    }
    MGINFO(m_description << " RPC server initialized OK on port: " << m_server.get_binded_port());
  }

  void run()
  {
    MGINFO("Starting " << m_description << " RPC server...");
    if (!m_server.run(2, false))
    {
      throw std::runtime_error("Failed to start " + m_description + " RPC server.");
    }
    MGINFO(m_description << " RPC server started ok");
  }

  void stop()
  {
    MGINFO("Stopping " << m_description << " RPC server...");
    m_server.send_stop_signal();
    m_server.timed_wait_server_stop(5000);
  }

  cryptonote::core_rpc_server* get_server()
  {
    return &m_server;
  }

  ~t_rpc()
  {
    MGINFO("Deinitializing " << m_description << " RPC server...");
    try {
      m_server.deinit();
    } catch (...) {
      MERROR("Failed to deinitialize " << m_description << " RPC server...");
    }
  }
};

}
