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

#pragma once

#include <string>
#include <system_error>

namespace CryptoNote {
namespace error {

enum class DataBaseErrorCodes : int {
  NOT_INITIALIZED = 1,
  ALREADY_INITIALIZED,
  INTERNAL_ERROR,
  IO_ERROR
};

class DataBaseErrorCategory : public std::error_category {
public:
  static DataBaseErrorCategory INSTANCE;

  virtual const char* name() const throw() override {
    return "DataBaseErrorCategory";
  }

  virtual std::error_condition default_error_condition(int ev) const throw() override {
    return std::error_condition(ev, *this);
  }

  virtual std::string message(int ev) const override {
    switch (ev) {
      case static_cast<int>(DataBaseErrorCodes::NOT_INITIALIZED) : return "Object was not initialized";
      case static_cast<int>(DataBaseErrorCodes::ALREADY_INITIALIZED) : return "Object has been already initialized";
      case static_cast<int>(DataBaseErrorCodes::INTERNAL_ERROR) : return "Internal error";
      case static_cast<int>(DataBaseErrorCodes::IO_ERROR) : return "IO error";
      default: return "Unknown error";
    }
  }

private:
  DataBaseErrorCategory() {

  }
};

} //namespace error
} //namespace CryptoNote

inline std::error_code make_error_code(CryptoNote::error::DataBaseErrorCodes e) {
  return std::error_code(static_cast<int>(e), CryptoNote::error::DataBaseErrorCategory::INSTANCE);
}
