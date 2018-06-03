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

#ifndef DAEMON_COMMAND_LINE_ARGS_H
#define DAEMON_COMMAND_LINE_ARGS_H

#include "common/command_line.h"
#include "cryptonote_config.h"
#include "daemonizer/daemonizer.h"

namespace daemon_args
{
  std::string const WINDOWS_SERVICE_NAME = "Monero Daemon";

  const command_line::arg_descriptor<std::string, false, true, 2> arg_config_file = {
    "config-file"
  , "Specify configuration file"
  , (daemonizer::get_default_data_dir() / std::string(CRYPTONOTE_NAME ".conf")).string()
  , {{ &cryptonote::arg_testnet_on, &cryptonote::arg_stagenet_on }}
  , [](std::array<bool, 2> testnet_stagenet, bool defaulted, std::string val)->std::string {
      if (testnet_stagenet[0] && defaulted)
        return (daemonizer::get_default_data_dir() / "testnet" /
                std::string(CRYPTONOTE_NAME ".conf")).string();
      else if (testnet_stagenet[1] && defaulted)
        return (daemonizer::get_default_data_dir() / "stagenet" /
                std::string(CRYPTONOTE_NAME ".conf")).string();
      return val;
    }
  };
  const command_line::arg_descriptor<std::string, false, true, 2> arg_log_file = {
    "log-file"
  , "Specify log file"
  , (daemonizer::get_default_data_dir() / std::string(CRYPTONOTE_NAME ".log")).string()
  , {{ &cryptonote::arg_testnet_on, &cryptonote::arg_stagenet_on }}
  , [](std::array<bool, 2> testnet_stagenet, bool defaulted, std::string val)->std::string {
      if (testnet_stagenet[0] && defaulted)
        return (daemonizer::get_default_data_dir() / "testnet" /
                std::string(CRYPTONOTE_NAME ".log")).string();
      else if (testnet_stagenet[1] && defaulted)
        return (daemonizer::get_default_data_dir() / "stagenet" /
                std::string(CRYPTONOTE_NAME ".log")).string();
      return val;
    }
  };
  const command_line::arg_descriptor<std::size_t> arg_max_log_file_size = {
    "max-log-file-size"
  , "Specify maximum log file size [B]"
  , MAX_LOG_FILE_SIZE
  };
  const command_line::arg_descriptor<std::string> arg_log_level = {
    "log-level"
  , ""
  , ""
  };
  const command_line::arg_descriptor<std::vector<std::string>> arg_command = {
    "daemon_command"
  , "Hidden"
  };
  const command_line::arg_descriptor<bool> arg_os_version = {
    "os-version"
  , "OS for which this executable was compiled"
  };
  const command_line::arg_descriptor<unsigned> arg_max_concurrency = {
    "max-concurrency"
  , "Max number of threads to use for a parallel job"
  , 0
  };

  const command_line::arg_descriptor<std::string> arg_zmq_rpc_bind_ip   = {
    "zmq-rpc-bind-ip"
      , "IP for ZMQ RPC server to listen on"
      , "127.0.0.1"
  };

  const command_line::arg_descriptor<std::string, false, true, 2> arg_zmq_rpc_bind_port = {
    "zmq-rpc-bind-port"
  , "Port for ZMQ RPC server to listen on"
  , std::to_string(config::ZMQ_RPC_DEFAULT_PORT)
  , {{ &cryptonote::arg_testnet_on, &cryptonote::arg_stagenet_on }}
  , [](std::array<bool, 2> testnet_stagenet, bool defaulted, std::string val)->std::string {
      if (testnet_stagenet[0] && defaulted)
        return std::to_string(config::testnet::ZMQ_RPC_DEFAULT_PORT);
      if (testnet_stagenet[1] && defaulted)
        return std::to_string(config::stagenet::ZMQ_RPC_DEFAULT_PORT);
      return val;
    }
  };

}  // namespace daemon_args

#endif // DAEMON_COMMAND_LINE_ARGS_H
