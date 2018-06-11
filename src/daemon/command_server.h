/**
@file
@details


Passing RPC commands:

@image html images/other/runtime-commands.png

*/

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

#include <boost/optional/optional_fwd.hpp>
#include "common/common_fwd.h"
#include "console_handler.h"
#include "daemon/command_parser_executor.h"

namespace daemonize {

class t_command_server {
private:
  t_command_parser_executor m_parser;
  epee::console_handlers_binder m_command_lookup;
  bool m_is_rpc;

public:
  t_command_server(
      uint32_t ip
    , uint16_t port
    , const boost::optional<tools::login>& login
    , bool is_rpc = true
    , cryptonote::core_rpc_server* rpc_server = NULL
    );

  bool process_command_str(const std::string& cmd);

  bool process_command_vec(const std::vector<std::string>& cmd);

  bool start_handling(std::function<void(void)> exit_handler = NULL);

  void stop_handling();

private:
  bool help(const std::vector<std::string>& args);

  std::string get_commands_str();
  std::string get_command_usage(const std::vector<std::string> &args);
};

} // namespace daemonize
