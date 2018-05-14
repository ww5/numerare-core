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

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "Common/int-util.h"
#include "crypto/hash.h"
#include "CryptoNoteConfig.h"
#include "Difficulty.h"

namespace CryptoNote {

	static bool cadd(uint64_t a, uint64_t b) { return a + b < a; }

	static bool cadc(uint64_t a, uint64_t b, bool c) { return a + b < a || (c && a + b == (uint64_t)-1); }

	bool check_hash(const Crypto::Hash &hash, Difficulty difficulty) {
		uint64_t low, high, top, cur;
		// First check the highest word, this will most likely fail for a random hash.
		top = mul128(swap64le(((const uint64_t *)&hash)[3]), difficulty, &high);
		if (high != 0) {
			return false;
		}
		
		low        = mul128(swap64le(((const uint64_t *)&hash)[0]), difficulty, &cur);  // TODO - low is not used
		low        = mul128(swap64le(((const uint64_t *)&hash)[1]), difficulty, &high);
		bool carry = cadd(cur, low);
		cur        = high;
		low        = mul128(swap64le(((const uint64_t *)&hash)[2]), difficulty, &high);
		carry      = cadc(cur, low, carry);
		carry      = cadc(high, top, carry);
		return !carry;
	}
}
