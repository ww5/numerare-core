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
#include "server.hpp"

// static initialisations
websocketpp::server<websocketpp::config::asio> WebsocketServer::server;
//std::map<std::string, websocketpp::connection_hdl> WebsocketServer::websockets;
std::vector<websocketpp::connection_hdl> WebsocketServer::websockets;
websocketpp::lib::shared_ptr<websocketpp::lib::thread> WebsocketServer::m_thread;
pthread_rwlock_t WebsocketServer::websocketsLock = PTHREAD_RWLOCK_INITIALIZER;
     
// namespace merging
using websocketpp::connection_hdl;

bool WebsocketServer::init() {
	//server.clear_access_channels(websocketpp::log::alevel::all);
    //server.clear_error_channels(websocketpp::log::elevel::all);

	// Initialising WebsocketServer.
	server.init_asio();

	// Register the message handlers.
	server.set_validate_handler(&WebsocketServer::on_validate);
	server.set_fail_handler(&WebsocketServer::on_fail);
	server.set_close_handler(&WebsocketServer::on_close);

	// Listen on port.
	int port = 9999;
	try {
		server.listen(port);
	} catch(websocketpp::exception const &e) {
		// Websocket exception on listen. Get char string via e.what().
	}

	// Starting Websocket accept.
	websocketpp::lib::error_code ec;
	server.start_accept(ec);
	if (ec) {
		// Can log an error message with the contents of ec.message() here.
		return false;
	}
	
	return true;
}

void WebsocketServer::run() {
    try {
		m_thread.reset(new websocketpp::lib::thread(&server::run, &server));         
    } catch(websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
    }
}

void WebsocketServer::stop() {
    // Stopping the Websocket listener and closing outstanding connections.
    websocketpp::lib::error_code ec;

    server.stop_listening(ec);
    if (ec) {
        // Failed to stop listening. Log reason using ec.message().
        return;
    }
     
    // Close all existing websocket connections.
    std::string data = "Terminating connection...";
    //std::map<std::string, websocketpp::connection_hdl>::iterator it;	
    for(auto const& id : websockets) 
  	{
        websocketpp::lib::error_code ec;
        server.close(id, websocketpp::close::status::normal, data, ec);//(it->second, websocketpp::close::status::normal, data, ec); // send text message.
        if (ec) { // we got an error
            // Error closing websocket. Log reason using ec.message().
        }
    }
     
    // Stop the endpoint.
    server.stop();
}

bool WebsocketServer::on_validate(connection_hdl hdl) {
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = server.get_con_from_hdl(hdl);
    websocketpp::uri_ptr uri = con->get_uri();
    /*std::string query = uri->get_query(); // returns empty string if no query string set.
    if (!query.empty()) {
        // Split the query parameter string here, if desired.
        // We assume we extracted a string called 'id' here.
		
    }
    else {
        // Reject if no query parameter provided, for example.
        return false;
    }*/
     
    if (pthread_rwlock_wrlock(&websocketsLock) != 0) {
        // Failed to write-lock websocketsLock.
    }
    	
    websockets.push_back(hdl); //std::pair<std::string, websocketpp::connection_hdl>(id, hdl));
    if (pthread_rwlock_unlock(&websocketsLock) != 0) {
        // Failed to unlock websocketsLock.
    }
 
    return true;
}

void WebsocketServer::on_fail(connection_hdl hdl) {
    websocketpp::server<websocketpp::config::asio>::connection_ptr con = server.get_con_from_hdl(hdl);
    // websocketpp::lib::error_code ec = con->get_ec();
    // Websocket connection attempt by client failed. Log reason using ec.message().
}
 
void WebsocketServer::on_close(connection_hdl hdl) {
    // Websocket connection closed.
	//websockets.erase(std::remove(websockets.begin(), websockets.end(), hdl), websockets.end()); 
}