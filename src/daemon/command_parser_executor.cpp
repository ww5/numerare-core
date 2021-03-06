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

#include "common/dns_utils.h"
#include "version.h"
#include "daemon/command_parser_executor.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize {

t_command_parser_executor::t_command_parser_executor(
    uint32_t ip
  , uint16_t port
  , const boost::optional<tools::login>& login
  , bool is_rpc
  , cryptonote::core_rpc_server* rpc_server
  )
  : m_executor(ip, port, login, is_rpc, rpc_server)
{}

bool t_command_parser_executor::print_peer_list(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.print_peer_list();
}

bool t_command_parser_executor::print_peer_list_stats(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.print_peer_list_stats();
}

bool t_command_parser_executor::save_blockchain(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.save_blockchain();
}

bool t_command_parser_executor::show_hash_rate(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.show_hash_rate();
}

bool t_command_parser_executor::hide_hash_rate(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.hide_hash_rate();
}

bool t_command_parser_executor::show_difficulty(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.show_difficulty();
}

bool t_command_parser_executor::show_status(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.show_status();
}

bool t_command_parser_executor::print_connections(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.print_connections();
}

bool t_command_parser_executor::print_blockchain_info(const std::vector<std::string>& args)
{
  if(!args.size())
  {
    std::cout << "need block index parameter" << std::endl;
    return false;
  }
  uint64_t start_index = 0;
  uint64_t end_index = 0;
  if(!epee::string_tools::get_xtype_from_string(start_index, args[0]))
  {
    std::cout << "wrong starter block index parameter" << std::endl;
    return false;
  }
  if(args.size() >1 && !epee::string_tools::get_xtype_from_string(end_index, args[1]))
  {
    std::cout << "wrong end block index parameter" << std::endl;
    return false;
  }

  return m_executor.print_blockchain_info(start_index, end_index);
}

bool t_command_parser_executor::set_log_level(const std::vector<std::string>& args)
{
  if(args.size() > 1)
  {
    std::cout << "use: set_log [<log_level_number_0-4> | <categories>]" << std::endl;
    return true;
  }

  if (args.empty())
  {
    return m_executor.set_log_categories("+");
  }

  uint16_t l = 0;
  if(epee::string_tools::get_xtype_from_string(l, args[0]))
  {
    if(4 < l)
    {
      std::cout << "wrong number range, use: set_log <log_level_number_0-4>" << std::endl;
      return true;
    }
    return m_executor.set_log_level(l);
  }
  else
  {
    return m_executor.set_log_categories(args.front());
  }
}

bool t_command_parser_executor::print_height(const std::vector<std::string>& args) 
{
  if (!args.empty()) return false;

  return m_executor.print_height();
}

bool t_command_parser_executor::print_block(const std::vector<std::string>& args)
{
  if (args.empty())
  {
    std::cout << "expected: print_block (<block_hash> | <block_height>)" << std::endl;
    return false;
  }

  const std::string& arg = args.front();
  try
  {
    uint64_t height = boost::lexical_cast<uint64_t>(arg);
    return m_executor.print_block_by_height(height);
  }
  catch (const boost::bad_lexical_cast&)
  {
    crypto::hash block_hash;
    if (parse_hash256(arg, block_hash))
    {
      return m_executor.print_block_by_hash(block_hash);
    }
  }

  return false;
}

bool t_command_parser_executor::print_transaction(const std::vector<std::string>& args)
{
  bool include_hex = false;
  bool include_json = false;

  // Assumes that optional flags come after mandatory argument <transaction_hash>
  for (unsigned int i = 1; i < args.size(); ++i) {
    if (args[i] == "+hex")
      include_hex = true;
    else if (args[i] == "+json")
      include_json = true;
    else
    {
      std::cout << "unexpected argument: " << args[i] << std::endl;
      return true;
    }
  }
  if (args.empty())
  {
    std::cout << "expected: print_tx <transaction_hash> [+hex] [+json]" << std::endl;
    return true;
  }

  const std::string& str_hash = args.front();
  crypto::hash tx_hash;
  if (parse_hash256(str_hash, tx_hash))
  {
    m_executor.print_transaction(tx_hash, include_hex, include_json);
  }

  return true;
}

bool t_command_parser_executor::is_key_image_spent(const std::vector<std::string>& args)
{
  if (args.empty())
  {
    std::cout << "expected: is_key_image_spent <key_image>" << std::endl;
    return true;
  }

  const std::string& str = args.front();
  crypto::key_image ki;
  crypto::hash hash;
  if (parse_hash256(str, hash))
  {
    memcpy(&ki, &hash, sizeof(ki));
    m_executor.is_key_image_spent(ki);
  }

  return true;
}

bool t_command_parser_executor::print_transaction_pool_long(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.print_transaction_pool_long();
}

bool t_command_parser_executor::print_transaction_pool_short(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.print_transaction_pool_short();
}

bool t_command_parser_executor::print_transaction_pool_stats(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.print_transaction_pool_stats();
}

bool t_command_parser_executor::start_mining(const std::vector<std::string>& args)
{
  if(!args.size())
  {
    std::cout << "Please specify a wallet address to mine for: start_mining <addr> [<threads>]" << std::endl;
    return true;
  }

  cryptonote::address_parse_info info;
  cryptonote::network_type nettype = cryptonote::MAINNET;
  if(!cryptonote::get_account_address_from_str(info, cryptonote::MAINNET, args.front()))
  {
    if(!cryptonote::get_account_address_from_str(info, cryptonote::TESTNET, args.front()))
    {
      if(!cryptonote::get_account_address_from_str(info, cryptonote::STAGENET, args.front()))
      {
        bool dnssec_valid;
        std::string address_str = tools::dns_utils::get_account_address_as_str_from_url(args.front(), dnssec_valid,
            [](const std::string &url, const std::vector<std::string> &addresses, bool dnssec_valid){return addresses[0];});
        if(!cryptonote::get_account_address_from_str(info, cryptonote::MAINNET, address_str))
        {
          if(!cryptonote::get_account_address_from_str(info, cryptonote::TESTNET, address_str))
          {
            if(!cryptonote::get_account_address_from_str(info, cryptonote::STAGENET, address_str))
            {
              std::cout << "target account address has wrong format" << std::endl;
              return true;
            }
            else
            {
              nettype = cryptonote::STAGENET;
            }
          }
          else
          {
            nettype = cryptonote::TESTNET;
          }
        }
      }
      else
      {
        nettype = cryptonote::STAGENET;
      }
    }
    else
    {
      nettype = cryptonote::TESTNET;
    }
  }

  if (info.is_subaddress)
  {
    tools::fail_msg_writer() << "subaddress for mining reward is not yet supported!" << std::endl;
    return true;
  }
  
  if(nettype != cryptonote::MAINNET)
    std::cout << "Mining to a " << (nettype == cryptonote::TESTNET ? "testnet" : "stagenet") << "address, make sure this is intentional!" << std::endl;

  uint64_t threads_count = 1;
  bool do_background_mining = false;  
  bool ignore_battery = false;  
  if(args.size() > 4)
  {
    return false;
  }
  
  std::string pool;
  if(args.back() == "pool" || args[(args.size() - 2)] == "pool") {
    if(args.size() < 4) {
      m_executor.print_pool_list(false);
      tools::success_msg_writer() << "select your server from the list:";
          
      std::cin >> pool; 
    } else {
      pool = args[3];
    }   
  } else {
    if(args.size() == 4)
    {
      ignore_battery = args[3] == "true";
    }  
    
    if(args.size() >= 3)
    {
      do_background_mining = args[2] == "true";
    }
  }
  
  if(args.size() >= 2)
  {
    bool ok = epee::string_tools::get_xtype_from_string(threads_count, args[1]);
    threads_count = (ok && 0 < threads_count) ? threads_count : 1;
  }

  m_executor.start_mining(info.address, threads_count, nettype, do_background_mining, ignore_battery, pool);

  return true;
}

bool t_command_parser_executor::stop_mining(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.stop_mining();
}

bool t_command_parser_executor::print_pool_list(const std::vector<std::string>& args)
{
  return m_executor.print_pool_list();
}

bool t_command_parser_executor::stop_daemon(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.stop_daemon();
}

bool t_command_parser_executor::print_status(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;

  return m_executor.print_status();
}

bool t_command_parser_executor::set_limit(const std::vector<std::string>& args)
{
  if(args.size()>1) return false;
  if(args.size()==0) {
    return m_executor.get_limit();
  }
  int64_t limit;
  try {
      limit = std::stoll(args[0]);
  }
  catch(const std::exception& ex) {
      std::cout << "failed to parse argument" << std::endl;
      return false;
  }

  return m_executor.set_limit(limit, limit);
}

bool t_command_parser_executor::set_limit_up(const std::vector<std::string>& args)
{
  if(args.size()>1) return false;
  if(args.size()==0) {
    return m_executor.get_limit_up();
  }
  int64_t limit;
  try {
      limit = std::stoll(args[0]);
  }
  catch(const std::exception& ex) {
      std::cout << "failed to parse argument" << std::endl;
      return false;
  }

  return m_executor.set_limit(0, limit);
}

bool t_command_parser_executor::set_limit_down(const std::vector<std::string>& args)
{
  if(args.size()>1) return false;
  if(args.size()==0) {
    return m_executor.get_limit_down();
  }
  int64_t limit;
  try {
      limit = std::stoll(args[0]);
  }
  catch(const std::exception& ex) {
      std::cout << "failed to parse argument" << std::endl;
      return false;
  }

  return m_executor.set_limit(limit, 0);
}

bool t_command_parser_executor::out_peers(const std::vector<std::string>& args)
{
	if (args.empty()) return false;
	
	unsigned int limit;
	try {
		limit = std::stoi(args[0]);
	}
	  
	catch(const std::exception& ex) {
		_erro("stoi exception");
		return false;
	}
	
	return m_executor.out_peers(limit);
}

bool t_command_parser_executor::in_peers(const std::vector<std::string>& args)
{
	if (args.empty()) return false;

	unsigned int limit;
	try {
		limit = std::stoi(args[0]);
	}

	catch(const std::exception& ex) {
		_erro("stoi exception");
		return false;
	}

	return m_executor.in_peers(limit);
}

bool t_command_parser_executor::start_save_graph(const std::vector<std::string>& args)
{
	if (!args.empty()) return false;
	return m_executor.start_save_graph();
}

bool t_command_parser_executor::stop_save_graph(const std::vector<std::string>& args)
{
	if (!args.empty()) return false;
	return m_executor.stop_save_graph();
}

bool t_command_parser_executor::hard_fork_info(const std::vector<std::string>& args)
{
  int version;
  if (args.size() == 0) {
    version = 0;
  }
  else if (args.size() == 1) {
    try {
      version = std::stoi(args[0]);
    }
    catch(const std::exception& ex) {
        return false;
    }
    if (version <= 0 || version > 255)
      return false;
  }
  else {
    return false;
  }
  return m_executor.hard_fork_info(version);
}

bool t_command_parser_executor::show_bans(const std::vector<std::string>& args)
{
  if (!args.empty()) return false;
  return m_executor.print_bans();
}

bool t_command_parser_executor::ban(const std::vector<std::string>& args)
{
  if (args.size() != 1 && args.size() != 2) return false;
  std::string ip = args[0];
  time_t seconds = P2P_IP_BLOCKTIME;
  if (args.size() > 1)
  {
    try
    {
      seconds = std::stoi(args[1]);
    }
    catch (const std::exception &e)
    {
      return false;
    }
    if (seconds == 0)
    {
      return false;
    }
  }
  return m_executor.ban(ip, seconds);
}

bool t_command_parser_executor::unban(const std::vector<std::string>& args)
{
  if (args.size() != 1) return false;
  std::string ip = args[0];
  return m_executor.unban(ip);
}

bool t_command_parser_executor::flush_txpool(const std::vector<std::string>& args)
{
  if (args.size() > 1) return false;

  std::string txid;
  if (args.size() == 1)
  {
    crypto::hash hash;
    if (!parse_hash256(args[0], hash))
    {
      std::cout << "failed to parse tx id" << std::endl;
      return true;
    }
    txid = args[0];
  }
  return m_executor.flush_txpool(txid);
}

bool t_command_parser_executor::output_histogram(const std::vector<std::string>& args)
{
  std::vector<uint64_t> amounts;
  uint64_t min_count = 3;
  uint64_t max_count = 0;
  size_t n_raw = 0;

  for (size_t n = 0; n < args.size(); ++n)
  {
    if (args[n][0] == '@')
    {
      amounts.push_back(boost::lexical_cast<uint64_t>(args[n].c_str() + 1));
    }
    else if (n_raw == 0)
    {
      min_count = boost::lexical_cast<uint64_t>(args[n]);
      n_raw++;
    }
    else if (n_raw == 1)
    {
      max_count = boost::lexical_cast<uint64_t>(args[n]);
      n_raw++;
    }
    else
    {
      std::cout << "Invalid syntax: more than two non-amount parameters" << std::endl;
      return true;
    }
  }
  return m_executor.output_histogram(amounts, min_count, max_count);
}

bool t_command_parser_executor::print_coinbase_tx_sum(const std::vector<std::string>& args)
{
  if(!args.size())
  {
    std::cout << "need block height parameter" << std::endl;
    return false;
  }
  uint64_t height = 0;
  uint64_t count = 0;
  if(!epee::string_tools::get_xtype_from_string(height, args[0]))
  {
    std::cout << "wrong starter block height parameter" << std::endl;
    return false;
  }
  if(args.size() >1 && !epee::string_tools::get_xtype_from_string(count, args[1]))
  {
    std::cout << "wrong count parameter" << std::endl;
    return false;
  }

  return m_executor.print_coinbase_tx_sum(height, count);
}

bool t_command_parser_executor::alt_chain_info(const std::vector<std::string>& args)
{
  if(args.size())
  {
    std::cout << "No parameters allowed" << std::endl;
    return false;
  }

  return m_executor.alt_chain_info();
}

bool t_command_parser_executor::print_blockchain_dynamic_stats(const std::vector<std::string>& args)
{
  if(args.size() != 1)
  {
    std::cout << "Exactly one parameter is needed" << std::endl;
    return false;
  }

  uint64_t nblocks = 0;
  if(!epee::string_tools::get_xtype_from_string(nblocks, args[0]) || nblocks == 0)
  {
    std::cout << "wrong number of blocks" << std::endl;
    return false;
  }

  return m_executor.print_blockchain_dynamic_stats(nblocks);
}

bool t_command_parser_executor::update(const std::vector<std::string>& args)
{
  if(args.size() != 1)
  {
    std::cout << "Exactly one parameter is needed: check, download, or update" << std::endl;
    return false;
  }

  return m_executor.update(args.front());
}

bool t_command_parser_executor::relay_tx(const std::vector<std::string>& args)
{
  if (args.size() != 1) return false;

  std::string txid;
  crypto::hash hash;
  if (!parse_hash256(args[0], hash))
  {
    std::cout << "failed to parse tx id" << std::endl;
    return true;
  }
  txid = args[0];
  return m_executor.relay_tx(txid);
}

bool t_command_parser_executor::sync_info(const std::vector<std::string>& args)
{
  if (args.size() != 0) return false;

  return m_executor.sync_info();
}

bool t_command_parser_executor::version(const std::vector<std::string>& args)
{
  std::cout << NUMERARE_STRING_VERSION << std::endl;
  return true;
}

} // namespace daemonize
