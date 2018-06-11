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

set(rpc_base_sources
  rpc_args.cpp)

set(rpc_sources
  core_rpc_server.cpp
  instanciations)

set(daemon_messages_sources
  message.cpp
  daemon_messages.cpp)

set(daemon_rpc_server_sources
  daemon_handler.cpp
  zmq_server.cpp)


set(rpc_base_headers
  rpc_args.h)

set(rpc_headers)

set(daemon_rpc_server_headers)


set(rpc_daemon_private_headers
  core_rpc_server.h
  core_rpc_server_commands_defs.h
  core_rpc_server_error_codes.h)

set(daemon_messages_private_headers
  message.h
  daemon_messages.h)

set(daemon_rpc_server_private_headers
  message.h
  daemon_messages.h
  daemon_handler.h
  rpc_handler.h
  zmq_server.h)


monero_private_headers(rpc
  ${rpc_private_headers})

monero_private_headers(daemon_rpc_server
  ${daemon_rpc_server_private_headers})


monero_add_library(rpc_base
  ${rpc_base_sources}
  ${rpc_base_headers}
  ${rpc_base_private_headers})

monero_add_library(rpc
  ${rpc_sources}
  ${rpc_headers}
  ${rpc_private_headers})

monero_add_library(daemon_messages
  ${daemon_messages_sources}
  ${daemon_messages_headers}
  ${daemon_messages_private_headers})

monero_add_library(daemon_rpc_server
  ${daemon_rpc_server_sources}
  ${daemon_rpc_server_headers}
  ${daemon_rpc_server_private_headers})


target_link_libraries(rpc_base
  PUBLIC
    common
    epee
    ${Boost_REGEX_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})

target_link_libraries(rpc
  PUBLIC
    rpc_base
    common
    cryptonote_core
    cryptonote_protocol
    epee
    ${Boost_REGEX_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})

target_link_libraries(daemon_messages
  LINK_PRIVATE
    cryptonote_core
    cryptonote_protocol
    serialization
    ${EXTRA_LIBRARIES})

target_link_libraries(daemon_rpc_server
  LINK_PRIVATE
    rpc
    cryptonote_core
    cryptonote_protocol
    daemon_messages
    serialization
    ${Boost_CHRONO_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${ZMQ_LIB}
    ${EXTRA_LIBRARIES})
target_include_directories(daemon_rpc_server PUBLIC ${ZMQ_INCLUDE_PATH})
target_include_directories(obj_daemon_rpc_server PUBLIC ${ZMQ_INCLUDE_PATH})
