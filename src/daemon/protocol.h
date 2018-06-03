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

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize
{

class t_protocol final
{
private:
  typedef cryptonote::t_cryptonote_protocol_handler<cryptonote::core> t_protocol_raw;
  typedef nodetool::node_server<t_protocol_raw> t_node_server;

  t_protocol_raw m_protocol;
public:
  t_protocol(
      boost::program_options::variables_map const & vm
    , t_core & core, bool offline = false
    )
    : m_protocol{core.get(), nullptr, offline}
  {
    MGINFO("Initializing cryptonote protocol...");
    if (!m_protocol.init(vm))
    {
      throw std::runtime_error("Failed to initialize cryptonote protocol.");
    }
    MGINFO("Cryptonote protocol initialized OK");
  }

  t_protocol_raw & get()
  {
    return m_protocol;
  }

  void set_p2p_endpoint(
      t_node_server & server
    )
  {
    m_protocol.set_p2p_endpoint(&server);
  }

  ~t_protocol()
  {
    MGINFO("Stopping cryptonote protocol...");
    try {
      m_protocol.deinit();
      m_protocol.set_p2p_endpoint(nullptr);
      MGINFO("Cryptonote protocol stopped successfully");
    } catch (...) {
      LOG_ERROR("Failed to stop cryptonote protocol!");
    }
  }
};

}
