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

enum class AddBlockErrorCode {
  ADDED_TO_MAIN = 1,
  ADDED_TO_ALTERNATIVE,
  ADDED_TO_ALTERNATIVE_AND_SWITCHED,
  ALREADY_EXISTS,
  REJECTED_AS_ORPHANED,
  DESERIALIZATION_FAILED
};

// custom category:
class AddBlockErrorCategory : public std::error_category {
public:
  static AddBlockErrorCategory INSTANCE;

  virtual const char* name() const throw() {
    return "AddBlockErrorCategory";
  }

  virtual std::error_condition default_error_condition(int ev) const throw() {
    return std::error_condition(ev, *this);
  }

  virtual std::string message(int ev) const {
    AddBlockErrorCode code = static_cast<AddBlockErrorCode>(ev);

    switch (code) {
      case AddBlockErrorCode::ADDED_TO_MAIN: return "Block added to main chain";
      case AddBlockErrorCode::ADDED_TO_ALTERNATIVE: return "Block added to alternative chain";
      case AddBlockErrorCode::ADDED_TO_ALTERNATIVE_AND_SWITCHED: return "Chain switched";
      case AddBlockErrorCode::ALREADY_EXISTS: return "Block already exists";
      case AddBlockErrorCode::REJECTED_AS_ORPHANED: return "Block rejected as orphaned";
      case AddBlockErrorCode::DESERIALIZATION_FAILED: return "Deserialization error";
      default: return "Unknown error";
    }
  }

private:
  AddBlockErrorCategory() {
  }
};

inline std::error_code make_error_code(CryptoNote::error::AddBlockErrorCode e) {
  return std::error_code(static_cast<int>(e), CryptoNote::error::AddBlockErrorCategory::INSTANCE);
}

}
}

namespace std {

template <>
struct is_error_code_enum<CryptoNote::error::AddBlockErrorCode>: public true_type {};

}
