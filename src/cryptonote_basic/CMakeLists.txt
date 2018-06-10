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

if(APPLE)
  find_library(IOKIT_LIBRARY IOKit)
  mark_as_advanced(IOKIT_LIBRARY)
  list(APPEND EXTRA_LIBRARIES ${IOKIT_LIBRARY})
endif()

set(cryptonote_basic_sources
  account.cpp
  cryptonote_basic_impl.cpp
  cryptonote_format_utils.cpp
  difficulty.cpp
  hardfork.cpp
  miner.cpp
  ../pool/client.cpp)

set(cryptonote_basic_headers)

set(cryptonote_basic_private_headers
  account.h
  account_boost_serialization.h
  connection_context.h
  cryptonote_basic.h
  cryptonote_basic_impl.h
  cryptonote_boost_serialization.h
  cryptonote_format_utils.h
  cryptonote_stat_info.h
  difficulty.h
  hardfork.h
  miner.h
  tx_extra.h
  verification_context.h
  ../pool/client.cpp)

monero_private_headers(cryptonote_basic
  ${cryptonote_basic_private_headers})
monero_add_library(cryptonote_basic
  ${cryptonote_basic_sources}
  ${cryptonote_basic_headers}
  ${cryptonote_basic_private_headers})
target_link_libraries(cryptonote_basic
  PUBLIC
    common
    cncrypto
    checkpoints
    device
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
