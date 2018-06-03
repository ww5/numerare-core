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

#pragma once

#include <sstream>
#include "binary_archive.h"

namespace serialization {
  /*! creates a new archive with the passed blob and serializes it into v
   */
  template <class T>
    bool parse_binary(const std::string &blob, T &v)
    {
      std::istringstream istr(blob);
      binary_archive<false> iar(istr);
      return ::serialization::serialize(iar, v);
    }

  /*! dumps the data in v into the blob string
   */
  template<class T>
    bool dump_binary(T& v, std::string& blob)
    {
      std::stringstream ostr;
      binary_archive<true> oar(ostr);
      bool success = ::serialization::serialize(oar, v);
      blob = ostr.str();
      return success && ostr.good();
    };

}