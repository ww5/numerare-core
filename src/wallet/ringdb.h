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

#include <string>
#include <vector>
#include <lmdb.h>
#include "wipeable_string.h"
#include "crypto/crypto.h"
#include "cryptonote_basic/cryptonote_basic.h"

namespace tools
{
  class ringdb
  {
  public:
    ringdb(std::string filename, const std::string &genesis);
    ~ringdb();

    bool add_rings(const crypto::chacha_key &chacha_key, const cryptonote::transaction_prefix &tx);
    bool remove_rings(const crypto::chacha_key &chacha_key, const cryptonote::transaction_prefix &tx);
    bool get_ring(const crypto::chacha_key &chacha_key, const crypto::key_image &key_image, std::vector<uint64_t> &outs);
    bool set_ring(const crypto::chacha_key &chacha_key, const crypto::key_image &key_image, const std::vector<uint64_t> &outs, bool relative);

    bool blackball(const crypto::public_key &output);
    bool unblackball(const crypto::public_key &output);
    bool blackballed(const crypto::public_key &output);
    bool clear_blackballs();

  private:
    bool blackball_worker(const crypto::public_key &output, int op);

  private:
    std::string filename;
    MDB_env *env;
    MDB_dbi dbi_rings;
    MDB_dbi dbi_blackballs;
  };
}
