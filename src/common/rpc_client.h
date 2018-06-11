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

#include "common/http_connection.h"
#include "common/scoped_message_writer.h"
#include "rpc/core_rpc_server_commands_defs.h"
#include "storages/http_abstract_invoke.h"
#include "net/http_auth.h"
#include "net/http_client.h"
#include "string_tools.h"

namespace tools
{
  class t_rpc_client final
  {
  private:
    epee::net_utils::http::http_simple_client m_http_client;
  public:
    t_rpc_client(
        uint32_t ip
      , uint16_t port
      , boost::optional<epee::net_utils::http::login> user
      )
      : m_http_client{}
    {
      m_http_client.set_server(
        epee::string_tools::get_ip_string_from_int32(ip), std::to_string(port), std::move(user)
      );
    }

    template <typename T_req, typename T_res>
    bool basic_json_rpc_request(
        T_req & req
      , T_res & res
      , std::string const & method_name
      )
    {
      t_http_connection connection(&m_http_client);

      bool ok = connection.is_open();
      if (!ok)
      {
        fail_msg_writer() << "Couldn't connect to daemon: " << m_http_client.get_host() << ":" << m_http_client.get_port();
        return false;
      }
      ok = epee::net_utils::invoke_http_json_rpc("/json_rpc", method_name, req, res, m_http_client, t_http_connection::TIMEOUT());
      if (!ok)
      {
        fail_msg_writer() << "basic_json_rpc_request: Daemon request failed";
        return false;
      }
      else
      {
        return true;
      }
    }

    template <typename T_req, typename T_res>
    bool json_rpc_request(
        T_req & req
      , T_res & res
      , std::string const & method_name
      , std::string const & fail_msg
      )
    {
      t_http_connection connection(&m_http_client);

      bool ok = connection.is_open();
      if (!ok)
      {
        fail_msg_writer() << "Couldn't connect to daemon: " << m_http_client.get_host() << ":" << m_http_client.get_port();
        return false;
      }
      ok = epee::net_utils::invoke_http_json_rpc("/json_rpc", method_name, req, res, m_http_client, t_http_connection::TIMEOUT());
      if (!ok || res.status != CORE_RPC_STATUS_OK) // TODO - handle CORE_RPC_STATUS_BUSY ?
      {
        fail_msg_writer() << fail_msg << " -- json_rpc_request: " << res.status;
        return false;
      }
      else
      {
        return true;
      }
    }

    template <typename T_req, typename T_res>
    bool rpc_request(
        T_req & req
      , T_res & res
      , std::string const & relative_url
      , std::string const & fail_msg
      )
    {
      t_http_connection connection(&m_http_client);

      bool ok = connection.is_open();
      if (!ok)
      {
        fail_msg_writer() << "Couldn't connect to daemon: " << m_http_client.get_host() << ":" << m_http_client.get_port();
        return false;
      }
      ok = epee::net_utils::invoke_http_json(relative_url, req, res, m_http_client, t_http_connection::TIMEOUT());
      if (!ok || res.status != CORE_RPC_STATUS_OK) // TODO - handle CORE_RPC_STATUS_BUSY ?
      {
        fail_msg_writer() << fail_msg << "-- rpc_request: " << res.status;
        return false;
      }
      else
      {
        return true;
      }
    }

    bool check_connection()
    {
      t_http_connection connection(&m_http_client);
      return connection.is_open();
    }
  };
}
