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

#include "LoggerMessage.h"

namespace Logging {

LoggerMessage::LoggerMessage(ILogger& logger, const std::string& category, Level level, const std::string& color)
  : std::ostream(this)
  , std::streambuf()
  , logger(logger)
  , category(category)
  , logLevel(level)
  , message(color)
  , timestamp(boost::posix_time::microsec_clock::local_time())
  , gotText(false) {
}

LoggerMessage::~LoggerMessage() {
  if (gotText) {
    (*this) << std::endl;
  }
}

#ifndef __linux__
LoggerMessage::LoggerMessage(LoggerMessage&& other)
  : std::ostream(std::move(other))
  , std::streambuf(std::move(other))
  , category(other.category)
  , logLevel(other.logLevel)
  , logger(other.logger)
  , message(other.message)
  , timestamp(boost::posix_time::microsec_clock::local_time())
  , gotText(false) {
  this->set_rdbuf(this);
}
#else
LoggerMessage::LoggerMessage(LoggerMessage&& other)
  : std::ostream(nullptr)
  , std::streambuf()
  , category(other.category)
  , logLevel(other.logLevel)
  , logger(other.logger)
  , message(other.message)
  , timestamp(boost::posix_time::microsec_clock::local_time())
  , gotText(false) {
  if (this != &other) {
    _M_tie = nullptr;
    _M_streambuf = nullptr;

    //ios_base swap
    std::swap(_M_streambuf_state, other._M_streambuf_state);
    std::swap(_M_exception, other._M_exception);
    std::swap(_M_flags, other._M_flags);
    std::swap(_M_precision, other._M_precision);
    std::swap(_M_width, other._M_width);

    std::swap(_M_callbacks, other._M_callbacks);
    std::swap(_M_ios_locale, other._M_ios_locale);
    //ios_base swap

    //streambuf swap
    char *_Pfirst = pbase();
    char *_Pnext = pptr();
    char *_Pend = epptr();
    char *_Gfirst = eback();
    char *_Gnext = gptr();
    char *_Gend = egptr();

    setp(other.pbase(), other.epptr());
    other.setp(_Pfirst, _Pend);

    setg(other.eback(), other.gptr(), other.egptr());
    other.setg(_Gfirst, _Gnext, _Gend);

    std::swap(_M_buf_locale, other._M_buf_locale);
    //streambuf swap

    std::swap(_M_fill, other._M_fill);
    std::swap(_M_tie, other._M_tie);
  }
  _M_streambuf = this;
}
#endif

int LoggerMessage::sync() {
  logger(category, logLevel, timestamp, message);
  gotText = false;
  message = DEFAULT;
  return 0;
}

std::streamsize LoggerMessage::xsputn(const char* s, std::streamsize n) {
  gotText = true;
  message.append(s, n);
  return n;
}

int LoggerMessage::overflow(int c) {
  gotText = true;
  message += static_cast<char>(c);
  return 0;
}

}
