/**
@file
@details

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

#include <boost/optional/optional.hpp>

#include "daemon/rpc_command_executor.h"
#include "common/common_fwd.h"
#include "rpc/core_rpc_server.h"

namespace daemonize {

class t_command_parser_executor final
{
private:
  t_rpc_command_executor m_executor;
public:
  t_command_parser_executor(
      uint32_t ip
    , uint16_t port
    , const boost::optional<tools::login>& login
    , bool is_rpc
    , cryptonote::core_rpc_server* rpc_server = NULL
    );

  bool print_peer_list(const std::vector<std::string>& args);

  bool print_peer_list_stats(const std::vector<std::string>& args);

  bool save_blockchain(const std::vector<std::string>& args);

  bool show_hash_rate(const std::vector<std::string>& args);

  bool hide_hash_rate(const std::vector<std::string>& args);

  bool show_difficulty(const std::vector<std::string>& args);

  bool show_status(const std::vector<std::string>& args);

  bool print_connections(const std::vector<std::string>& args);

  bool print_blockchain_info(const std::vector<std::string>& args);

  bool set_log_level(const std::vector<std::string>& args);

  bool set_log_categories(const std::vector<std::string>& args);

  bool print_height(const std::vector<std::string>& args);

  bool print_block(const std::vector<std::string>& args);

  bool print_transaction(const std::vector<std::string>& args);

  bool is_key_image_spent(const std::vector<std::string>& args);

  bool print_transaction_pool_long(const std::vector<std::string>& args);

  bool print_transaction_pool_short(const std::vector<std::string>& args);

  bool print_transaction_pool_stats(const std::vector<std::string>& args);

  bool start_mining(const std::vector<std::string>& args);

  bool stop_mining(const std::vector<std::string>& args);

  bool print_pool_list(const std::vector<std::string>& args);

  bool stop_daemon(const std::vector<std::string>& args);

  bool print_status(const std::vector<std::string>& args);

  bool set_limit(const std::vector<std::string>& args);

  bool set_limit_up(const std::vector<std::string>& args);

  bool set_limit_down(const std::vector<std::string>& args);

  bool out_peers(const std::vector<std::string>& args);

  bool in_peers(const std::vector<std::string>& args);

  bool start_save_graph(const std::vector<std::string>& args);
  
  bool stop_save_graph(const std::vector<std::string>& args);
  
  bool hard_fork_info(const std::vector<std::string>& args);

  bool show_bans(const std::vector<std::string>& args);

  bool ban(const std::vector<std::string>& args);

  bool unban(const std::vector<std::string>& args);

  bool flush_txpool(const std::vector<std::string>& args);

  bool output_histogram(const std::vector<std::string>& args);

  bool print_coinbase_tx_sum(const std::vector<std::string>& args);

  bool alt_chain_info(const std::vector<std::string>& args);

  bool print_blockchain_dynamic_stats(const std::vector<std::string>& args);

  bool update(const std::vector<std::string>& args);

  bool relay_tx(const std::vector<std::string>& args);

  bool sync_info(const std::vector<std::string>& args);

  bool version(const std::vector<std::string>& args);
};

} // namespace daemonize
