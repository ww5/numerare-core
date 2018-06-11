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

#include <boost/uuid/uuid.hpp>
#include "net/net_utils_base.h"
#include "p2p_protocol_defs.h"

namespace nodetool
{

  typedef boost::uuids::uuid uuid;
  typedef boost::uuids::uuid net_connection_id;

  template<class t_connection_context>
  struct i_p2p_endpoint
  {
    virtual bool relay_notify_to_list(int command, const std::string& data_buff, const std::list<boost::uuids::uuid>& connections)=0;
    virtual bool relay_notify_to_all(int command, const std::string& data_buff, const epee::net_utils::connection_context_base& context)=0;
    virtual bool invoke_command_to_peer(int command, const std::string& req_buff, std::string& resp_buff, const epee::net_utils::connection_context_base& context)=0;
    virtual bool invoke_notify_to_peer(int command, const std::string& req_buff, const epee::net_utils::connection_context_base& context)=0;
    virtual bool drop_connection(const epee::net_utils::connection_context_base& context)=0;
    virtual void request_callback(const epee::net_utils::connection_context_base& context)=0;
    virtual uint64_t get_connections_count()=0;
    virtual void for_each_connection(std::function<bool(t_connection_context&, peerid_type, uint32_t)> f)=0;
    virtual bool for_connection(const boost::uuids::uuid&, std::function<bool(t_connection_context&, peerid_type, uint32_t)> f)=0;
    virtual bool block_host(const epee::net_utils::network_address &address, time_t seconds = 0)=0;
    virtual bool unblock_host(const epee::net_utils::network_address &address)=0;
    virtual std::map<std::string, time_t> get_blocked_hosts()=0;
    virtual bool add_host_fail(const epee::net_utils::network_address &address)=0;
  };

  template<class t_connection_context>
  struct p2p_endpoint_stub: public i_p2p_endpoint<t_connection_context>
  {
    virtual bool relay_notify_to_list(int command, const std::string& data_buff, const std::list<boost::uuids::uuid>& connections)
    {
      return false;
    }
    virtual bool relay_notify_to_all(int command, const std::string& data_buff, const epee::net_utils::connection_context_base& context)
    {
      return false;
    }
    virtual bool invoke_command_to_peer(int command, const std::string& req_buff, std::string& resp_buff, const epee::net_utils::connection_context_base& context)
    {
      return false;
    }
    virtual bool invoke_notify_to_peer(int command, const std::string& req_buff, const epee::net_utils::connection_context_base& context)
    {
      return true;
    }
    virtual bool drop_connection(const epee::net_utils::connection_context_base& context)
    {
      return false;
    }
    virtual void request_callback(const epee::net_utils::connection_context_base& context)
    {

    }
    virtual void for_each_connection(std::function<bool(t_connection_context&,peerid_type,uint32_t)> f)
    {

    }
    virtual bool for_connection(const boost::uuids::uuid&, std::function<bool(t_connection_context&,peerid_type,uint32_t)> f)
    {
      return false;
    }

    virtual uint64_t get_connections_count()    
    {
      return false;
    }
    virtual bool block_host(const epee::net_utils::network_address &address, time_t seconds)
    {
      return true;
    }
    virtual bool unblock_host(const epee::net_utils::network_address &address)
    {
      return true;
    }
    virtual std::map<std::string, time_t> get_blocked_hosts()
    {
      return std::map<std::string, time_t>();
    }
    virtual bool add_host_fail(const epee::net_utils::network_address &address)
    {
      return true;
    }
  };
}
