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

include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})

set(common_sources
  base58.cpp
  command_line.cpp
  dns_utils.cpp
  download.cpp
  util.cpp
  i18n.cpp
  password.cpp
  perf_timer.cpp
  threadpool.cpp
  updates.cpp)

if (STACK_TRACE)
  list(APPEND common_sources stack_trace.cpp)
endif()

set(common_headers)

set(common_private_headers
  apply_permutation.h
  base58.h
  boost_serialization_helper.h
  command_line.h
  common_fwd.h
  dns_utils.h
  download.h
  http_connection.h
  int-util.h
  pod-class.h
  rpc_client.h
  scoped_message_writer.h
  unordered_containers_boost_serialization.h
  util.h
  varint.h
  i18n.h
  password.h
  perf_timer.h
  stack_trace.h
  threadpool.h
  updates.h)

monero_private_headers(common
  ${common_private_headers})
monero_add_library(common
  ${common_sources}
  ${common_headers}
  ${common_private_headers}
  DEPENDS generate_translations_header)
target_link_libraries(common
  PUBLIC
    epee
    cncrypto
    ${UNBOUND_LIBRARY}
    ${LIBUNWIND_LIBRARIES}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
  PRIVATE
    ${OPENSSL_LIBRARIES}
    ${EXTRA_LIBRARIES})

#monero_install_headers(common
#  ${common_headers})
