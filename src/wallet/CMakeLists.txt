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

set(wallet_sources
  wallet2.cpp
  wallet_args.cpp
  ringdb.cpp
  node_rpc_proxy.cpp)

set(wallet_private_headers
  wallet2.h
  wallet_args.h
  wallet_errors.h
  wallet_rpc_server.h
  wallet_rpc_server_commands_defs.h
  wallet_rpc_server_error_codes.h
  ringdb.h
  node_rpc_proxy.h)

monero_private_headers(wallet
  ${wallet_private_headers})
monero_add_library(wallet
  ${wallet_sources}
  ${wallet_private_headers})
target_link_libraries(wallet
  PUBLIC
    multisig
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

set(wallet_rpc_sources
  wallet_rpc_server.cpp)

set(wallet_rpc_headers)

set(wallet_rpc_private_headers
  wallet_rpc_server.h)

monero_private_headers(wallet_rpc_server
  ${wallet_rpc_private_headers})
monero_add_executable(wallet_rpc_server
  ${wallet_rpc_sources}
  ${wallet_rpc_headers}
  ${wallet_rpc_private_headers})

target_link_libraries(wallet_rpc_server
  PRIVATE
    wallet
    epee
    rpc_base
    cryptonote_core
    cncrypto
    common
    version
    ${Boost_CHRONO_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${EXTRA_LIBRARIES})
set_property(TARGET wallet_rpc_server
  PROPERTY
    OUTPUT_NAME "wallet-rpc")
install(TARGETS wallet_rpc_server DESTINATION bin)


# build and install libwallet_merged only if we building for GUI
if (BUILD_GUI_DEPS)
    set(libs_to_merge
            wallet_api
            wallet
            multisig
            cryptonote_core
            cryptonote_basic
            mnemonics
            common
            cncrypto
            device
            ringct
            ringct_basic
            checkpoints
            version)

    foreach(lib ${libs_to_merge})
        list(APPEND objlibs $<TARGET_OBJECTS:obj_${lib}>) # matches naming convention in src/CMakeLists.txt
    endforeach()
    add_library(wallet_merged STATIC ${objlibs})
    if(IOS)
        set(lib_folder lib-${ARCH})
    else()
        set(lib_folder lib)
    endif()
    install(TARGETS wallet_merged
        ARCHIVE DESTINATION ${lib_folder})
endif()

add_subdirectory(api)
