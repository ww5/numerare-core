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

#include <vector>

#include "serialization.h"
#include "debug_archive.h"
#include "crypto/chacha.h"
#include "crypto/crypto.h"
#include "crypto/hash.h"

// read
template <template <bool> class Archive>
bool do_serialize(Archive<false> &ar, std::vector<crypto::signature> &v)
{
  size_t cnt = v.size();
  v.clear();

  // very basic sanity check
  if (ar.remaining_bytes() < cnt*sizeof(crypto::signature)) {
    ar.stream().setstate(std::ios::failbit);
    return false;
  }

  v.reserve(cnt);
  for (size_t i = 0; i < cnt; i++) {
    v.resize(i+1);
    ar.serialize_blob(&(v[i]), sizeof(crypto::signature), "");
    if (!ar.stream().good())
      return false;
  }
  return true;
}

// write
template <template <bool> class Archive>
bool do_serialize(Archive<true> &ar, std::vector<crypto::signature> &v)
{
  if (0 == v.size()) return true;
  ar.begin_string();
  size_t cnt = v.size();
  for (size_t i = 0; i < cnt; i++) {
    ar.serialize_blob(&(v[i]), sizeof(crypto::signature), "");
    if (!ar.stream().good())
      return false;
  }
  ar.end_string();
  return true;
}

BLOB_SERIALIZER(crypto::chacha_iv);
BLOB_SERIALIZER(crypto::hash);
BLOB_SERIALIZER(crypto::hash8);
BLOB_SERIALIZER(crypto::public_key);
BLOB_SERIALIZER(crypto::secret_key);
BLOB_SERIALIZER(crypto::key_derivation);
BLOB_SERIALIZER(crypto::key_image);
BLOB_SERIALIZER(crypto::signature);
VARIANT_TAG(debug_archive, crypto::hash, "hash");
VARIANT_TAG(debug_archive, crypto::hash8, "hash8");
VARIANT_TAG(debug_archive, crypto::public_key, "public_key");
VARIANT_TAG(debug_archive, crypto::secret_key, "secret_key");
VARIANT_TAG(debug_archive, crypto::key_derivation, "key_derivation");
VARIANT_TAG(debug_archive, crypto::key_image, "key_image");
VARIANT_TAG(debug_archive, crypto::signature, "signature");
