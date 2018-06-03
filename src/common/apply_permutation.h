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
// 
// Most of this file is originally copyright (c) 2017 Raymond Chen, Microsoft
// This algorithm is adapted from Raymond Chen's code:
// https://blogs.msdn.microsoft.com/oldnewthing/20170109-00/?p=95145

#pragma once

#include <vector>
#include <functional>
#include "misc_log_ex.h"

namespace tools
{

template<typename F>
void apply_permutation(std::vector<size_t> permutation, const F &swap)
{
  //sanity check
  for (size_t n = 0; n < permutation.size(); ++n)
    CHECK_AND_ASSERT_THROW_MES(std::find(permutation.begin(), permutation.end(), n) != permutation.end(), "Bad permutation");

  for (size_t i = 0; i < permutation.size(); ++i)
  {
    size_t current = i;
    while (i != permutation[current])
    {
      size_t next = permutation[current];
      swap(current, next);
      permutation[current] = current;
      current = next;
    }
    permutation[current] = current;
  }
}

template<typename T>
void apply_permutation(const std::vector<size_t> &permutation, std::vector<T> &v)
{
  CHECK_AND_ASSERT_THROW_MES(permutation.size() == v.size(), "Mismatched vector sizes");
  apply_permutation(permutation, [&v](size_t i0, size_t i1){ std::swap(v[i0], v[i1]); });
}

}
