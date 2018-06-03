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

set(cn_deserialize_sources
  cn_deserialize.cpp
  )

monero_add_executable(cn_deserialize
  ${cn_deserialize_sources}
  ${cn_deserialize_private_headers})

target_link_libraries(cn_deserialize
  LINK_PRIVATE
    cryptonote_core
    blockchain_db
    device
    ringct
    p2p
    epee
    ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET cn_deserialize
	PROPERTY
	OUTPUT_NAME "monero-utils-deserialize")


set(object_sizes_sources
  object_sizes.cpp
  )

monero_add_executable(object_sizes
  ${object_sizes_sources}
  ${object_sizes_private_headers})

target_link_libraries(object_sizes
  LINK_PRIVATE
    cryptonote_core
    blockchain_db
    p2p
    epee
    ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET object_sizes
	PROPERTY
	OUTPUT_NAME "monero-utils-object-sizes")

