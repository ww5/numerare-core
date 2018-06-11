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

#include "net/net_utils_base.h"
#include "p2p/p2p_protocol_defs.h"

namespace boost
{
  namespace serialization
  {
    template <class T, class Archive>
    inline void do_serialize(Archive &a, epee::net_utils::network_address& na, T local)
    {
      if (typename Archive::is_saving()) local = na.as<T>();
      a & local;
      if (!typename Archive::is_saving()) na = local;
    }
    template <class Archive, class ver_type>
    inline void serialize(Archive &a, epee::net_utils::network_address& na, const ver_type ver)
    {
      uint8_t type;
      if (typename Archive::is_saving())
        type = na.get_type_id();
      a & type;
      switch (type)
      {
        case epee::net_utils::ipv4_network_address::ID:
	  do_serialize(a, na, epee::net_utils::ipv4_network_address{0, 0});
	  break; 
        default:
          throw std::runtime_error("Unsupported network address type");
      }
    }
    template <class Archive, class ver_type>
    inline void serialize(Archive &a, epee::net_utils::ipv4_network_address& na, const ver_type ver)
    {
      uint32_t ip{na.ip()};
      uint16_t port{na.port()};
      a & ip;
      a & port;
      if (!typename Archive::is_saving())
        na = epee::net_utils::ipv4_network_address{ip, port};
    }

    template <class Archive, class ver_type>
    inline void serialize(Archive &a,  nodetool::peerlist_entry& pl, const ver_type ver)
    {
      a & pl.adr;
      a & pl.id;
      a & pl.last_seen;
    }

    template <class Archive, class ver_type>
    inline void serialize(Archive &a, nodetool::anchor_peerlist_entry& pl, const ver_type ver)
    {
      a & pl.adr;
      a & pl.id;
      a & pl.first_seen;
    }
  }
}
