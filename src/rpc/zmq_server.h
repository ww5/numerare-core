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

#include <boost/thread/thread.hpp>
#include <zmq.hpp>
#include <string>
#include <memory>

#include "common/command_line.h"

#include "rpc_handler.h"

namespace cryptonote
{

namespace rpc
{

static constexpr int DEFAULT_NUM_ZMQ_THREADS = 1;
static constexpr int DEFAULT_RPC_RECV_TIMEOUT_MS = 1000;

class ZmqServer
{
  public:

    ZmqServer(RpcHandler& h);

    ~ZmqServer();

    static void init_options(boost::program_options::options_description& desc);

    void serve();

    bool addIPCSocket(std::string address, std::string port);
    bool addTCPSocket(std::string address, std::string port);

    void run();
    void stop();

  private:
    RpcHandler& handler;

    volatile bool stop_signal;
    volatile bool running;

    zmq::context_t context;

    boost::thread run_thread;

    std::unique_ptr<zmq::socket_t> rep_socket;
};


}  // namespace cryptonote

}  // namespace rpc
