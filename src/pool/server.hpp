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
	
	Thanks to Maya Posch for the basic code of class implementation.
	@https://mayaposch.wordpress.com/2015/09/16/creating-a-websocket-server-with-websocket/
***/
#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

class WebsocketServer {
	public:
		static bool init();
		static void run();
		static void stop();
	
		static bool sendClose(std::string id);
		static bool sendData(std::string id, std::string data);
			
	private:
		static bool getWebsocket(const std::string &id, websocketpp::connection_hdl &hdl);
		
		static websocketpp::server<websocketpp::config::asio> server;
		static pthread_rwlock_t websocketsLock;
		static websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
		//static std::map<std::string, websocketpp::connection_hdl> websockets;
		static std::vector<websocketpp::connection_hdl> websockets;
					
		// callbacks
		//static void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
		static bool on_validate(websocketpp::connection_hdl hdl);
		static void on_fail(websocketpp::connection_hdl hdl);
		static void on_close(websocketpp::connection_hdl hdl);
};