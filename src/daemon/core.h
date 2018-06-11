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

#include "cryptonote_core/cryptonote_core.h"
#include "cryptonote_protocol/cryptonote_protocol_handler.h"
#include "misc_log_ex.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize
{

class t_core final
{
public:
  static void init_options(boost::program_options::options_description & option_spec)
  {
    cryptonote::core::init_options(option_spec);
  }
private:
  typedef cryptonote::t_cryptonote_protocol_handler<cryptonote::core> t_protocol_raw;
  cryptonote::core m_core;
  // TEMPORARY HACK - Yes, this creates a copy, but otherwise the original
  // variable map could go out of scope before the run method is called
  boost::program_options::variables_map const m_vm_HACK;
public:
  t_core(
      boost::program_options::variables_map const & vm
    )
    : m_core{nullptr}
    , m_vm_HACK{vm}
  {
  }

  // TODO - get rid of circular dependencies in internals
  void set_protocol(t_protocol_raw & protocol)
  {
    m_core.set_cryptonote_protocol(&protocol);
  }

  std::string get_config_subdir() const
  {
    bool testnet = command_line::get_arg(m_vm_HACK, cryptonote::arg_testnet_on);
    bool stagenet = command_line::get_arg(m_vm_HACK, cryptonote::arg_stagenet_on);
    bool mainnet = !testnet && !stagenet;
    std::string port = command_line::get_arg(m_vm_HACK, nodetool::arg_p2p_bind_port);
    if ((mainnet && port != std::to_string(::config::P2P_DEFAULT_PORT))
        || (testnet && port != std::to_string(::config::testnet::P2P_DEFAULT_PORT))
        || (stagenet && port != std::to_string(::config::stagenet::P2P_DEFAULT_PORT))) {
      return port;
    }
    return std::string();
  }

  bool run()
  {
    //initialize core here
    MGINFO("Initializing core...");
    std::string config_subdir = get_config_subdir();
    if (!m_core.init(m_vm_HACK, config_subdir.empty() ? NULL : config_subdir.c_str()))
    {
      return false;
    }
    MGINFO("Core initialized OK");
    return true;
  }

  cryptonote::core & get()
  {
    return m_core;
  }

  ~t_core()
  {
    MGINFO("Deinitializing core...");
    try {
      m_core.deinit();
      m_core.set_cryptonote_protocol(nullptr);
    } catch (...) {
      MERROR("Failed to deinitialize core...");
    }
  }
};

}
