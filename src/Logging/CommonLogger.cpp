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

#include "CommonLogger.h"

namespace Logging {

namespace {

std::string formatPattern(const std::string& pattern, const std::string& category, Level level, boost::posix_time::ptime time) {
  std::stringstream s;

  for (const char* p = pattern.c_str(); p && *p != 0; ++p) {
    if (*p == '%') {
      ++p;
      switch (*p) {
      case 0:
        break;
      case 'C':
        s << category;
        break;
      case 'D':
        s << time.date();
        break;
      case 'T':
        s << time.time_of_day();
        break;
      case 'L':
        s << std::setw(7) << std::left << ILogger::LEVEL_NAMES[level];
        break;
      default:
        s << *p;
      }
    } else {
      s << *p;
    }
  }

  return s.str();
}

}

void CommonLogger::operator()(const std::string& category, Level level, boost::posix_time::ptime time, const std::string& body) {
  if (level <= logLevel && disabledCategories.count(category) == 0) {
    std::string body2 = body;
    if (!pattern.empty()) {
      size_t insertPos = 0;
      if (!body2.empty() && body2[0] == ILogger::COLOR_DELIMETER) {
        size_t delimPos = body2.find(ILogger::COLOR_DELIMETER, 1);
        if (delimPos != std::string::npos) {
          insertPos = delimPos + 1;
        }
      }

      body2.insert(insertPos, formatPattern(pattern, category, level, time));
    }

    doLogString(body2);
  }
}

void CommonLogger::setPattern(const std::string& pattern) {
  this->pattern = pattern;
}

void CommonLogger::enableCategory(const std::string& category) {
  disabledCategories.erase(category);
}

void CommonLogger::disableCategory(const std::string& category) {
  disabledCategories.insert(category);
}

void CommonLogger::setMaxLevel(Level level) {
  logLevel = level;
}

CommonLogger::CommonLogger(Level level) : logLevel(level), pattern("%D %T %L [%C] ") {
}

void CommonLogger::doLogString(const std::string& message) {
}

}
