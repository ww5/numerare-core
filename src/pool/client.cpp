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
#include "client.hpp"

websocketpp::client<websocketpp::config::asio_client> WebsocketClient::c;
websocketpp::connection_hdl WebsocketClient::m_con;
websocketpp::lib::shared_ptr<websocketpp::lib::thread> WebsocketClient::m_thread;
websocketpp::lib::error_code ec;

void WebsocketClient::init() {
    try {
        c.clear_access_channels(websocketpp::log::alevel::all);
        c.clear_error_channels(websocketpp::log::elevel::all);

        c.init_asio();

        c.start_perpetual();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }    
}

bool WebsocketClient::run(std::string uri) {
    try {
        c::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return false;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);

        m_con = con->get_handle();

        m_thread.reset(new websocketpp::lib::thread(&c::run, &c));  
        
        //std::cout << "connection started..." << std::endl;
        return true; 
    } catch(websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
        return false;
    }

    return false;
}

void WebsocketClient::stop() {
    try {
        c.close(m_con, websocketpp::close::status::going_away, "", ec);
        if (ec) {
            std::cout << "Error initiating close: " << ec.message() << std::endl;
        }

        //std::cout << "connection closed..." << std::endl;
    } catch(websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
    } 
}

void WebsocketClient::uninit() {
    try {
        c.stop_perpetual();    
        m_thread->join();

        //std::cout << "close client..." << std::endl;
    } catch(websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
    } 
}