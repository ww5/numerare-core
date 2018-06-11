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

#include "serialization/keyvalue_serialization.h"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <ostream>

namespace cryptonote
{
  struct subaddress_index
  {
    uint32_t major;
    uint32_t minor;
    bool operator==(const subaddress_index& rhs) const { return !memcmp(this, &rhs, sizeof(subaddress_index)); }
    bool operator!=(const subaddress_index& rhs) const { return !(*this == rhs); }
    bool is_zero() const { return major == 0 && minor == 0; }

    BEGIN_SERIALIZE_OBJECT()
      FIELD(major)
      FIELD(minor)
    END_SERIALIZE()

    BEGIN_KV_SERIALIZE_MAP()
      KV_SERIALIZE(major)
      KV_SERIALIZE(minor)
    END_KV_SERIALIZE_MAP()
  };
}

namespace cryptonote {
  inline std::ostream& operator<<(std::ostream& out, const cryptonote::subaddress_index& subaddr_index)
  {
    return out << subaddr_index.major << '/' << subaddr_index.minor;
  }
}

namespace std
{
  template <>
  struct hash<cryptonote::subaddress_index>
  {
    size_t operator()(const cryptonote::subaddress_index& index ) const
    {
      size_t res;
      if (sizeof(size_t) == 8)
      {
        res = ((uint64_t)index.major << 32) | index.minor;
      }
      else
      {
        // https://stackoverflow.com/a/17017281
        res = 17;
        res = res * 31 + hash<uint32_t>()(index.major);
        res = res * 31 + hash<uint32_t>()(index.minor);
      }
      return res;
    }
  };
}

BOOST_CLASS_VERSION(cryptonote::subaddress_index, 0)

namespace boost
{
  namespace serialization
  {
    template <class Archive>
    inline void serialize(Archive &a, cryptonote::subaddress_index &x, const boost::serialization::version_type ver)
    {
      a & x.major;
      a & x.minor;
    }
  }
}
