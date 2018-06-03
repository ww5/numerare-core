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
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <string>

#include "common/command_line.h"
#include "common/password.h"

namespace cryptonote
{
  //! Processes command line arguments related to server-side RPC
  struct rpc_args
  {
    // non-static construction prevents initialization order issues
    struct descriptors
    {
      descriptors();
      descriptors(const descriptors&) = delete;
      descriptors(descriptors&&) = delete;
      descriptors& operator=(const descriptors&) = delete;
      descriptors& operator=(descriptors&&) = delete;

      const command_line::arg_descriptor<std::string> rpc_bind_ip;
      const command_line::arg_descriptor<std::string> rpc_login;
      const command_line::arg_descriptor<bool> confirm_external_bind;
      const command_line::arg_descriptor<std::string> rpc_access_control_origins;
    };

    static const char* tr(const char* str);
    static void init_options(boost::program_options::options_description& desc);

    //! \return Arguments specified by user, or `boost::none` if error
    static boost::optional<rpc_args> process(const boost::program_options::variables_map& vm);

    std::string bind_ip;
    std::vector<std::string> access_control_origins;
    boost::optional<tools::login> login; // currently `boost::none` if unspecified by user
  };
}
