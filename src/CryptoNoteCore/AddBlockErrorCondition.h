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

#include "AddBlockErrors.h"
#include "BlockValidationErrors.h"
#include "TransactionValidationErrors.h"

namespace CryptoNote {
namespace error {

enum class AddBlockErrorCondition {
  BLOCK_ADDED = 0,
  BLOCK_REJECTED,
  BLOCK_VALIDATION_FAILED,
  TRANSACTION_VALIDATION_FAILED,
  DESERIALIZATION_FAILED
};

class AddBlockErrorConditionCategory: public std::error_category {
public:
  static AddBlockErrorConditionCategory INSTANCE;

  virtual const char* name() const throw() override {
    return "AddBlockErrorCondition";
  }

  virtual std::error_condition default_error_condition(int ev) const throw() override {
    return std::error_condition(ev, *this);
  }

  virtual std::string message(int ev) const override {
    AddBlockErrorCondition code = static_cast<AddBlockErrorCondition>(ev);

    switch (code) {
      case AddBlockErrorCondition::BLOCK_ADDED: return "Block successfully added";
      case AddBlockErrorCondition::BLOCK_REJECTED: return "Block rejected";
      case AddBlockErrorCondition::BLOCK_VALIDATION_FAILED: return "Block validation failed";
      case AddBlockErrorCondition::TRANSACTION_VALIDATION_FAILED: return "Transaction validation failed";
      default: return "Unknown error condition";
    }
  }

  virtual bool equivalent(const std::error_code& errorCode, int condition) const throw() override  {
    AddBlockErrorCondition code = static_cast<AddBlockErrorCondition>(condition);

    switch(code) {
      case AddBlockErrorCondition::BLOCK_ADDED:
        return errorCode == AddBlockErrorCode::ADDED_TO_MAIN
          || errorCode == AddBlockErrorCode::ADDED_TO_ALTERNATIVE
          || errorCode == AddBlockErrorCode::ADDED_TO_ALTERNATIVE_AND_SWITCHED
          || errorCode == AddBlockErrorCode::ALREADY_EXISTS;

      case AddBlockErrorCondition::DESERIALIZATION_FAILED:
        return errorCode == AddBlockErrorCode::DESERIALIZATION_FAILED;

      case AddBlockErrorCondition::BLOCK_REJECTED:
        return errorCode == AddBlockErrorCode::REJECTED_AS_ORPHANED;

      case AddBlockErrorCondition::BLOCK_VALIDATION_FAILED:
        return &errorCode.category() == &BlockValidationErrorCategory::INSTANCE;

      case AddBlockErrorCondition::TRANSACTION_VALIDATION_FAILED:
        return &errorCode.category() == &TransactionValidationErrorCategory::INSTANCE;

      default: return false;
    }
  }
};

std::error_condition make_error_condition(AddBlockErrorCondition e);

}
}

namespace std {

template <>
struct is_error_condition_enum<CryptoNote::error::AddBlockErrorCondition>: public true_type {};

}
