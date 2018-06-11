#
#	MIT License
#
#	Copyright (c) 2018 NUMERARE
#
#	Permission is hereby granted, free of charge, to any person obtaining a copy
#	of this software and associated documentation files (the "Software"), to dea###l
#	in the Software without restriction, including without limitation the rights
#	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#	copies of the Software, and to permit persons to whom the Software is
#	furnished to do so, subject to the following conditions:
#
#	The above copyright notice and this permission notice shall be included in all
#	copies or substantial portions of the Software.
#
#	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#	SOFTWARE.
#
#	Parts of this file are originally Copyright (c) 2012-2014 The CryptoNote developers,
#	                                                2014-2018 The Monero Project
#

set(ringct_basic_sources
  rctOps.cpp
  rctTypes.cpp
  rctCryptoOps.c
  bulletproofs.cc)

set(ringct_basic_private_headers
  rctOps.h
  rctTypes.h
  bulletproofs.h)

monero_private_headers(ringct_basic
  ${crypto_private_headers})
monero_add_library(ringct_basic
  ${ringct_basic_sources}
  ${ringct_basic_private_headers})
target_link_libraries(ringct_basic
  PUBLIC
    common
    cncrypto
  PRIVATE
    ${OPENSSL_LIBRARIES}
    ${EXTRA_LIBRARIES})

set(ringct_sources
  rctSigs.cpp
)

set(ringct_headers)

set(ringct_private_headers
  rctSigs.h
)

monero_private_headers(ringct
  ${crypto_private_headers})
monero_add_library(ringct
  ${ringct_sources}
  ${ringct_headers}
  ${ringct_private_headers})
target_link_libraries(ringct
  PUBLIC
    common
    cncrypto
    cryptonote_basic
    device
  PRIVATE
    ${OPENSSL_LIBRARIES}
    ${EXTRA_LIBRARIES})
