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

#include <memory>
#include <stdexcept>
#include <boost/algorithm/string/split.hpp>
#include "misc_log_ex.h"
#include "daemon/daemon.h"
#include "rpc/daemon_handler.h"
#include "rpc/zmq_server.h"

#include "common/password.h"
#include "common/util.h"
#include "daemon/core.h"
#include "daemon/p2p.h"
#include "daemon/protocol.h"
#include "daemon/rpc.h"
#include "daemon/command_server.h"
#include "daemon/command_server.h"
#include "daemon/command_line_args.h"
#include "version.h"

using namespace epee;

#include <functional>

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "daemon"

namespace daemonize {

struct t_internals {
private:
  t_protocol protocol;
public:
  t_core core;
  t_p2p p2p;
  std::vector<std::unique_ptr<t_rpc>> rpcs;

  t_internals(
      boost::program_options::variables_map const & vm
    )
    : core{vm}
    , protocol{vm, core, command_line::get_arg(vm, cryptonote::arg_offline)}
    , p2p{vm, protocol}
  {
    // Handle circular dependencies
    protocol.set_p2p_endpoint(p2p.get());
    core.set_protocol(protocol.get());

    const auto testnet = command_line::get_arg(vm, cryptonote::arg_testnet_on);
    const auto stagenet = command_line::get_arg(vm, cryptonote::arg_stagenet_on);
    const auto restricted = command_line::get_arg(vm, cryptonote::core_rpc_server::arg_restricted_rpc);
    const auto main_rpc_port = command_line::get_arg(vm, cryptonote::core_rpc_server::arg_rpc_bind_port);
    rpcs.emplace_back(new t_rpc{vm, core, p2p, restricted, testnet ? cryptonote::TESTNET : stagenet ? cryptonote::STAGENET : cryptonote::MAINNET, main_rpc_port, "core"});

    auto restricted_rpc_port_arg = cryptonote::core_rpc_server::arg_rpc_restricted_bind_port;
    if(!command_line::is_arg_defaulted(vm, restricted_rpc_port_arg))
    {
      auto restricted_rpc_port = command_line::get_arg(vm, restricted_rpc_port_arg);
      rpcs.emplace_back(new t_rpc{vm, core, p2p, true, testnet ? cryptonote::TESTNET : stagenet ? cryptonote::STAGENET : cryptonote::MAINNET, restricted_rpc_port, "restricted"});
    }
  }
};

void t_daemon::init_options(boost::program_options::options_description & option_spec)
{
  t_core::init_options(option_spec);
  t_p2p::init_options(option_spec);
  t_rpc::init_options(option_spec);
}

t_daemon::t_daemon(
    boost::program_options::variables_map const & vm
  )
  : mp_internals{new t_internals{vm}}
{
  zmq_rpc_bind_port = command_line::get_arg(vm, daemon_args::arg_zmq_rpc_bind_port);
  zmq_rpc_bind_address = command_line::get_arg(vm, daemon_args::arg_zmq_rpc_bind_ip);
}

t_daemon::~t_daemon() = default;

// MSVC is brain-dead and can't default this...
t_daemon::t_daemon(t_daemon && other)
{
  if (this != &other)
  {
    mp_internals = std::move(other.mp_internals);
    other.mp_internals.reset(nullptr);
  }
}

// or this
t_daemon & t_daemon::operator=(t_daemon && other)
{
  if (this != &other)
  {
    mp_internals = std::move(other.mp_internals);
    other.mp_internals.reset(nullptr);
  }
  return *this;
}

bool t_daemon::run(bool interactive)
{
  if (nullptr == mp_internals)
  {
    throw std::runtime_error{"Can't run stopped daemon"};
  }
  tools::signal_handler::install(std::bind(&daemonize::t_daemon::stop_p2p, this));

  try
  {
    if (!mp_internals->core.run())
      return false;

    for(auto& rpc: mp_internals->rpcs)
      rpc->run();

    std::unique_ptr<daemonize::t_command_server> rpc_commands;
    if (interactive && mp_internals->rpcs.size())
    {
      // The first three variables are not used when the fourth is false
      rpc_commands.reset(new daemonize::t_command_server(0, 0, boost::none, false, mp_internals->rpcs.front()->get_server()));
      rpc_commands->start_handling(std::bind(&daemonize::t_daemon::stop_p2p, this));
    }

    cryptonote::rpc::DaemonHandler rpc_daemon_handler(mp_internals->core.get(), mp_internals->p2p.get());
    cryptonote::rpc::ZmqServer zmq_server(rpc_daemon_handler);

    if (!zmq_server.addTCPSocket(zmq_rpc_bind_address, zmq_rpc_bind_port))
    {
      LOG_ERROR(std::string("Failed to add TCP Socket (") + zmq_rpc_bind_address
          + ":" + zmq_rpc_bind_port + ") to ZMQ RPC Server");

      if (rpc_commands)
        rpc_commands->stop_handling();

      for(auto& rpc : mp_internals->rpcs)
        rpc->stop();

      return false;
    }

    MINFO("Starting ZMQ server...");
    zmq_server.run();

    MINFO(std::string("ZMQ server started at ") + zmq_rpc_bind_address
          + ":" + zmq_rpc_bind_port + ".");

    mp_internals->p2p.run(); // blocks until p2p goes down

    if (rpc_commands)
      rpc_commands->stop_handling();

    zmq_server.stop();

    for(auto& rpc : mp_internals->rpcs)
      rpc->stop();
    mp_internals->core.get().get_miner().stop();
    MGINFO("Node stopped.");
    return true;
  }
  catch (std::exception const & ex)
  {
    MFATAL("Uncaught exception! " << ex.what());
    return false;
  }
  catch (...)
  {
    MFATAL("Uncaught exception!");
    return false;
  }
}

void t_daemon::stop()
{
  if (nullptr == mp_internals)
  {
    throw std::runtime_error{"Can't stop stopped daemon"};
  }
  mp_internals->core.get().get_miner().stop();
  mp_internals->p2p.stop();
  for(auto& rpc : mp_internals->rpcs)
    rpc->stop();

  mp_internals.reset(nullptr); // Ensure resources are cleaned up before we return
}

void t_daemon::stop_p2p()
{
  if (nullptr == mp_internals)
  {
    throw std::runtime_error{"Can't send stop signal to a stopped daemon"};
  }
  mp_internals->p2p.get().send_stop_signal();
}

} // namespace daemonize
