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

#include "HttpRequest.h"

namespace CryptoNote {

  const std::string& HttpRequest::getMethod() const {
    return method;
  }

  const std::string& HttpRequest::getUrl() const {
    return url;
  }

  const HttpRequest::Headers& HttpRequest::getHeaders() const {
    return headers;
  }

  const std::string& HttpRequest::getBody() const {
    return body;
  }

  void HttpRequest::addHeader(const std::string& name, const std::string& value) {
    headers[name] = value;
  }
  void HttpRequest::setBody(const std::string& b) {
    body = b;
    if (!body.empty()) {
      headers["Content-Length"] = std::to_string(body.size());
    }
    else {
      headers.erase("Content-Length");
    }
  }

  void HttpRequest::setUrl(const std::string& u) {
    url = u;
  }

  std::ostream& HttpRequest::printHttpRequest(std::ostream& os) const {
    os << "POST " << url << " HTTP/1.1\r\n";
    auto host = headers.find("Host");
    if (host == headers.end()) {
      os << "Host: " << "127.0.0.1" << "\r\n";
    }

    for (auto pair : headers) {
      os << pair.first << ": " << pair.second << "\r\n";
    }
    
    os << "\r\n";
    if (!body.empty()) {
      os << body;
    }

    return os;
  }
}
