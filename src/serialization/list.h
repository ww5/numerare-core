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

#include <list>

template <template <bool> class Archive, class T>
bool do_serialize(Archive<false> &ar, std::list<T> &v);
template <template <bool> class Archive, class T>
bool do_serialize(Archive<true> &ar, std::list<T> &v);

namespace serialization
{
  namespace detail
  {
    template <typename T>
    void do_add(std::list<T> &c, T &&e)
    {
      c.emplace_back(std::move(e));
    }
  }
}

#include "serialization.h"

template <template <bool> class Archive, class T>
bool do_serialize(Archive<false> &ar, std::list<T> &v) { return do_serialize_container(ar, v); }
template <template <bool> class Archive, class T>
bool do_serialize(Archive<true> &ar, std::list<T> &v) { return do_serialize_container(ar, v); }

