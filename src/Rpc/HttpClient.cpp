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

	Parts of this file are originally Copyright (c) 2012-2017 The CryptoNote developers, The Bytecoin developers
***/

#include "HttpClient.h"

#include <HTTP/HttpParser.h>
#include <System/Ipv4Resolver.h>
#include <System/Ipv4Address.h>
#include <System/TcpConnector.h>

namespace CryptoNote {

HttpClient::HttpClient(System::Dispatcher& dispatcher, const std::string& address, uint16_t port) :
  m_dispatcher(dispatcher), m_address(address), m_port(port) {
}

HttpClient::~HttpClient() {
  if (m_connected) {
    disconnect();
  }
}

void HttpClient::request(const HttpRequest &req, HttpResponse &res) {
  if (!m_connected) {
    connect();
  }

  try {
    std::iostream stream(m_streamBuf.get());
    HttpParser parser;
    stream << req;
    stream.flush();
    parser.receiveResponse(stream, res);
  } catch (const std::exception &) {
    disconnect();
    throw;
  }
}

void HttpClient::connect() {
  try {
    auto ipAddr = System::Ipv4Resolver(m_dispatcher).resolve(m_address);
    m_connection = System::TcpConnector(m_dispatcher).connect(ipAddr, m_port);
    m_streamBuf.reset(new System::TcpStreambuf(m_connection));
    m_connected = true;
  } catch (const std::exception& e) {
    throw ConnectException(e.what());
  }
}

bool HttpClient::isConnected() const {
  return m_connected;
}

void HttpClient::disconnect() {
  m_streamBuf.reset();
  try {
    m_connection.write(nullptr, 0); //Socket shutdown.
  } catch (std::exception&) {
    //Ignoring possible exception.
  }

  try {
    m_connection = System::TcpConnection();
  } catch (std::exception&) {
    //Ignoring possible exception.
  }

  m_connected = false;
}

ConnectException::ConnectException(const std::string& whatArg) : std::runtime_error(whatArg.c_str()) {
}

}
