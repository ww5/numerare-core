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

# include (${PROJECT_SOURCE_DIR}/cmake/libutils.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(wallet_api_sources
  wallet.cpp
  wallet_manager.cpp
  transaction_info.cpp
  transaction_history.cpp
  pending_transaction.cpp
  utils.cpp
  address_book.cpp
  subaddress.cpp
  subaddress_account.cpp
  unsigned_transaction.cpp)

set(wallet_api_headers
    wallet2_api.h)

set(wallet_api_private_headers
  wallet.h
  wallet_manager.h
  transaction_info.h
  transaction_history.h
  pending_transaction.h
  common_defines.h
  address_book.h
  subaddress.h
  subaddress_account.h
  unsigned_transaction.h)

monero_private_headers(wallet_api
  ${wallet_api_private_headers})
monero_add_library(wallet_api
  ${wallet_api_sources}
  ${wallet_api_headers}
  ${wallet_api_private_headers})
target_link_libraries(wallet_api
  PUBLIC
    wallet
    common
    cryptonote_core
    mnemonics
    ${LMDB_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_REGEX_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})

set_property(TARGET wallet_api PROPERTY EXCLUDE_FROM_ALL TRUE)
set_property(TARGET obj_wallet_api PROPERTY EXCLUDE_FROM_ALL TRUE)

if(IOS)
    set(lib_folder lib-${ARCH})
else()
    set(lib_folder lib)
endif()

install(FILES ${wallet_api_headers}
    DESTINATION include/wallet/api)
