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

#include "serialization.h"

namespace serialization
{
  namespace detail
  {
    template <typename Archive, class T>
    bool serialize_container_element(Archive& ar, T& e)
    {
      return ::do_serialize(ar, e);
    }

    template <typename Archive>
    bool serialize_container_element(Archive& ar, uint32_t& e)
    {
      ar.serialize_varint(e);
      return true;
    }

    template <typename Archive>
    bool serialize_container_element(Archive& ar, uint64_t& e)
    {
      ar.serialize_varint(e);
      return true;
    }

    template <typename C>
    void do_reserve(C &c, size_t N) {}
  }
}

template <template <bool> class Archive, typename C>
bool do_serialize_container(Archive<false> &ar, C &v)
{
  size_t cnt;
  ar.begin_array(cnt);
  if (!ar.stream().good())
    return false;
  v.clear();

  // very basic sanity check
  if (ar.remaining_bytes() < cnt) {
    ar.stream().setstate(std::ios::failbit);
    return false;
  }

  ::serialization::detail::do_reserve(v, cnt);

  for (size_t i = 0; i < cnt; i++) {
    if (i > 0)
      ar.delimit_array();
    typename C::value_type e;
    if (!::serialization::detail::serialize_container_element(ar, e))
      return false;
    ::serialization::detail::do_add(v, std::move(e));
    if (!ar.stream().good())
      return false;
  }
  ar.end_array();
  return true;
}

template <template <bool> class Archive, typename C>
bool do_serialize_container(Archive<true> &ar, C &v)
{
  size_t cnt = v.size();
  ar.begin_array(cnt);
  for (auto i = v.begin(); i != v.end(); ++i)
  {
    if (!ar.stream().good())
      return false;
    if (i != v.begin())
      ar.delimit_array();
    if(!::serialization::detail::serialize_container_element(ar, const_cast<typename C::value_type&>(*i)))
      return false;
    if (!ar.stream().good())
      return false;
  }
  ar.end_array();
  return true;
}
