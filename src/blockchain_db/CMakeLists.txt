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

set(blockchain_db_sources
  blockchain_db.cpp
  lmdb/db_lmdb.cpp
  )

if (BERKELEY_DB)
  set(blockchain_db_sources
  ${blockchain_db_sources}
  berkeleydb/db_bdb.cpp
  )
endif()


set(blockchain_db_headers)

set(blockchain_db_private_headers
  blockchain_db.h
  lmdb/db_lmdb.h
  )

if (BERKELEY_DB)
  set(blockchain_db_private_headers
  ${blockchain_db_private_headers}
  berkeleydb/db_bdb.h
  )
endif()

monero_private_headers(blockchain_db
  ${crypto_private_headers})
monero_add_library(blockchain_db
  ${blockchain_db_sources}
  ${blockchain_db_headers}
  ${blockchain_db_private_headers})
target_link_libraries(blockchain_db
  PUBLIC
    common
    cncrypto
    ringct
    ${LMDB_LIBRARY}
    ${BDB_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
