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
//
#include "rpc_args.h"

#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include "common/command_line.h"
#include "common/i18n.h"

namespace cryptonote
{
  rpc_args::descriptors::descriptors()
     : rpc_bind_ip({"rpc-bind-ip", rpc_args::tr("Specify IP to bind RPC server"), "127.0.0.1"})
     , rpc_login({"rpc-login", rpc_args::tr("Specify username[:password] required for RPC server"), "", true})
     , confirm_external_bind({"confirm-external-bind", rpc_args::tr("Confirm rpc-bind-ip value is NOT a loopback (local) IP")})
     , rpc_access_control_origins({"rpc-access-control-origins", rpc_args::tr("Specify a comma separated list of origins to allow cross origin resource sharing"), ""})
  {}

  const char* rpc_args::tr(const char* str) { return i18n_translate(str, "cryptonote::rpc_args"); }

  void rpc_args::init_options(boost::program_options::options_description& desc)
  {
    const descriptors arg{};
    command_line::add_arg(desc, arg.rpc_bind_ip);
    command_line::add_arg(desc, arg.rpc_login);
    command_line::add_arg(desc, arg.confirm_external_bind);
    command_line::add_arg(desc, arg.rpc_access_control_origins);
  }

  boost::optional<rpc_args> rpc_args::process(const boost::program_options::variables_map& vm)
  {
    const descriptors arg{};
    rpc_args config{};
    
    config.bind_ip = command_line::get_arg(vm, arg.rpc_bind_ip);
    if (!config.bind_ip.empty())
    {
      // always parse IP here for error consistency
      boost::system::error_code ec{};
      const auto parsed_ip = boost::asio::ip::address::from_string(config.bind_ip, ec);
      if (ec)
      {
        LOG_ERROR(tr("Invalid IP address given for --") << arg.rpc_bind_ip.name);
        return boost::none;
      }

      if (!parsed_ip.is_loopback() && !command_line::get_arg(vm, arg.confirm_external_bind))
      {
        LOG_ERROR(
          "--" << arg.rpc_bind_ip.name <<
          tr(" permits inbound unencrypted external connections. Consider SSH tunnel or SSL proxy instead. Override with --") <<
          arg.confirm_external_bind.name
        );
        return boost::none;
      }
    }

    if (command_line::has_arg(vm, arg.rpc_login))
    {
      config.login = tools::login::parse(command_line::get_arg(vm, arg.rpc_login), true, [](bool verify) {
        return tools::password_container::prompt(verify, "RPC server password");
      });
      if (!config.login)
        return boost::none;

      if (config.login->username.empty())
      {
        LOG_ERROR(tr("Username specified with --") << arg.rpc_login.name << tr(" cannot be empty"));
        return boost::none;
      }
    }

    auto access_control_origins_input = command_line::get_arg(vm, arg.rpc_access_control_origins);
    if (!access_control_origins_input.empty())
    {
      if (!config.login)
      {
        LOG_ERROR(arg.rpc_access_control_origins.name  << tr(" requires RFC server password --") << arg.rpc_login.name << tr(" cannot be empty"));
        return boost::none;
      }

      std::vector<std::string> access_control_origins;
      boost::split(access_control_origins, access_control_origins_input, boost::is_any_of(","));
      std::for_each(access_control_origins.begin(), access_control_origins.end(), boost::bind(&boost::trim<std::string>, _1, std::locale::classic()));
      config.access_control_origins = std::move(access_control_origins);
    }

    return {std::move(config)};
  }
}
