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

#include "HttpParser.h"

#include <algorithm>

#include "HttpParserErrorCodes.h"

namespace {

void throwIfNotGood(std::istream& stream) {
  if (!stream.good()) {
    if (stream.eof()) {
      throw std::system_error(make_error_code(CryptoNote::error::HttpParserErrorCodes::END_OF_STREAM));
    } else {
      throw std::system_error(make_error_code(CryptoNote::error::HttpParserErrorCodes::STREAM_NOT_GOOD));
    }
  }
}

}

namespace CryptoNote {

HttpResponse::HTTP_STATUS HttpParser::parseResponseStatusFromString(const std::string& status) {
  if (status == "200 OK" || status == "200 Ok") return CryptoNote::HttpResponse::STATUS_200;
  else if (status == "404 Not Found") return CryptoNote::HttpResponse::STATUS_404;
  else if (status == "500 Internal Server Error") return CryptoNote::HttpResponse::STATUS_500;
  else throw std::system_error(make_error_code(CryptoNote::error::HttpParserErrorCodes::UNEXPECTED_SYMBOL),
      "Unknown HTTP status code is given");

  return CryptoNote::HttpResponse::STATUS_200; //unaccessible
}


void HttpParser::receiveRequest(std::istream& stream, HttpRequest& request) {
  readWord(stream, request.method);
  readWord(stream, request.url);

  std::string httpVersion;
  readWord(stream, httpVersion);

  readHeaders(stream, request.headers);

  std::string body;
  size_t bodyLen = getBodyLen(request.headers);
  if (bodyLen) {
    readBody(stream, request.body, bodyLen);
  }
}


void HttpParser::receiveResponse(std::istream& stream, HttpResponse& response) {
  std::string httpVersion;
  readWord(stream, httpVersion);
  
  std::string status;
  char c;
  
  stream.get(c);
  while (stream.good() && c != '\r') { //Till the end
    status += c;
    stream.get(c);
  }

  throwIfNotGood(stream);

  if (c == '\r') {
    stream.get(c);
    if (c != '\n') {
      throw std::runtime_error("Parser error: '\\n' symbol is expected");
    }
  }

  response.setStatus(parseResponseStatusFromString(status));
  
  std::string name;
  std::string value;

  while (readHeader(stream, name, value)) {
    response.addHeader(name, value);
    name.clear();
    value.clear();
  }

  response.addHeader(name, value);
  auto headers = response.getHeaders();
  size_t length = 0;
  auto it = headers.find("content-length");
  if (it != headers.end()) {
    length = std::stoul(it->second);
  }
  
  std::string body;
  if (length) {
    readBody(stream, body, length);
  }

  response.setBody(body);
}


void HttpParser::readWord(std::istream& stream, std::string& word) {
  char c;

  stream.get(c);
  while (stream.good() && c != ' ' && c != '\r') {
    word += c;
    stream.get(c);
  }

  throwIfNotGood(stream);

  if (c == '\r') {
    stream.get(c);
    if (c != '\n') {
      throw std::system_error(make_error_code(CryptoNote::error::HttpParserErrorCodes::UNEXPECTED_SYMBOL));
    }
  }
}

void HttpParser::readHeaders(std::istream& stream, HttpRequest::Headers& headers) {
  std::string name;
  std::string value;

  while (readHeader(stream, name, value)) {
    headers[name] = value; //use insert
    name.clear();
    value.clear();
  }

  headers[name] = value; //use insert
}

bool HttpParser::readHeader(std::istream& stream, std::string& name, std::string& value) {
  char c;
  bool isName = true;

  stream.get(c);
  while (stream.good() && c != '\r') {
    if (c == ':') {
      if (stream.peek() == ' ') {
        stream.get(c);
      }

      if (name.empty()) {
        throw std::system_error(make_error_code(CryptoNote::error::HttpParserErrorCodes::EMPTY_HEADER));
      }

      if (isName) {
        isName = false;
        stream.get(c);
        continue;
      }
    }

    if (isName) {
      name += c;
      stream.get(c);
    } else {
      value += c;
      stream.get(c);
    }
  }

  throwIfNotGood(stream);

  stream.get(c);
  if (c != '\n') {
    throw std::system_error(make_error_code(CryptoNote::error::HttpParserErrorCodes::UNEXPECTED_SYMBOL));
  }

  std::transform(name.begin(), name.end(), name.begin(), ::tolower);

  c = stream.peek();
  if (c == '\r') {
    stream.get(c).get(c);
    if (c != '\n') {
      throw std::system_error(make_error_code(CryptoNote::error::HttpParserErrorCodes::UNEXPECTED_SYMBOL));
    }

    return false; //no more headers
  }

  return true;
}

size_t HttpParser::getBodyLen(const HttpRequest::Headers& headers) {
  auto it = headers.find("content-length");
  if (it != headers.end()) {
    size_t bytes = std::stoul(it->second);
    return bytes;
  }

  return 0;
}

void HttpParser::readBody(std::istream& stream, std::string& body, const size_t bodyLen) {
  size_t read = 0;

  while (stream.good() && read < bodyLen) {
    body += stream.get();
    ++read;
  }

  throwIfNotGood(stream);
}

}
