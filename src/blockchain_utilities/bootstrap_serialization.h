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

#include "cryptonote_basic/cryptonote_boost_serialization.h"
#include "cryptonote_basic/difficulty.h"


namespace cryptonote
{
  namespace bootstrap
  {

    struct file_info
    {
      uint8_t  major_version;
      uint8_t  minor_version;
      uint32_t header_size;

      BEGIN_SERIALIZE_OBJECT()
        FIELD(major_version);
        FIELD(minor_version);
        VARINT_FIELD(header_size);
      END_SERIALIZE()
    };

    struct blocks_info
    {
      // block heights of file's first and last blocks, zero-based indexes
      uint64_t block_first;
      uint64_t block_last;

      // file position, for directly reading last block
      uint64_t block_last_pos;

      BEGIN_SERIALIZE_OBJECT()
        VARINT_FIELD(block_first);
        VARINT_FIELD(block_last);
        VARINT_FIELD(block_last_pos);
      END_SERIALIZE()
    };

    struct block_package
    {
      cryptonote::block block;
      std::vector<transaction> txs;
      size_t block_size;
      difficulty_type cumulative_difficulty;
      uint64_t coins_generated;

      BEGIN_SERIALIZE()
        FIELD(block)
        FIELD(txs)
        VARINT_FIELD(block_size)
        VARINT_FIELD(cumulative_difficulty)
        VARINT_FIELD(coins_generated)
      END_SERIALIZE()
    };

  }

}
