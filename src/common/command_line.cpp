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

#include "command_line.h"
#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <unordered_set>
#include "common/i18n.h"
#include "cryptonote_config.h"
#include "string_tools.h"

namespace command_line
{
  namespace
  {
    const char* tr(const char* str)
    {
      return i18n_translate(str, "command_line");      
    }
  }

  bool is_yes(const std::string& str)
  {
    if (str == "y" || str == "Y")
      return true;

    boost::algorithm::is_iequal ignore_case{};
    if (boost::algorithm::equals("yes", str, ignore_case))
      return true;
    if (boost::algorithm::equals(command_line::tr("yes"), str, ignore_case))
      return true;

    return false;
  }

  bool is_no(const std::string& str)
  {
    if (str == "n" || str == "N")
      return true;

    boost::algorithm::is_iequal ignore_case{};
    if (boost::algorithm::equals("no", str, ignore_case))
      return true;
    if (boost::algorithm::equals(command_line::tr("no"), str, ignore_case))
      return true;

    return false;
  }

  const arg_descriptor<bool> arg_help = {"help", "Produce help message"};
  const arg_descriptor<bool> arg_version = {"version", "Output version information"};
}
