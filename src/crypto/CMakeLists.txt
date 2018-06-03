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

set(crypto_sources
  aesb.c
  blake256.c
  chacha.c
  crypto-ops-data.c
  crypto-ops.c
  crypto.cpp
  groestl.c
  hash-extra-blake.c
  hash-extra-groestl.c
  hash-extra-jh.c
  hash-extra-skein.c
  hash.c
  jh.c
  keccak.c
  oaes_lib.c
  random.c
  skein.c
  slow-hash.c
  tree-hash.c)

set(crypto_headers)

set(crypto_private_headers
  blake256.h
  chacha.h
  crypto-ops.h
  crypto.h
  generic-ops.h
  groestl.h
  groestl_tables.h
  hash-ops.h
  hash.h
  initializer.h
  jh.h
  keccak.h
  oaes_config.h
  oaes_lib.h
  random.h
  skein.h
  skein_port.h)

monero_private_headers(cncrypto
  ${crypto_private_headers})
monero_add_library(cncrypto
  ${crypto_sources}
  ${crypto_headers}
  ${crypto_private_headers})
target_link_libraries(cncrypto
  PUBLIC
    epee
    ${Boost_SYSTEM_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})

if (ARM)
  option(NO_OPTIMIZED_MULTIPLY_ON_ARM
	   "Compute multiply using generic C implementation instead of ARM ASM" OFF)
  if(NO_OPTIMIZED_MULTIPLY_ON_ARM)
    message(STATUS "Using generic C implementation for multiply")
    set_property(SOURCE slow-hash.c
      PROPERTY COMPILE_DEFINITIONS "NO_OPTIMIZED_MULTIPLY_ON_ARM")
  endif()
endif()

# Because of the way Qt works on android with JNI, the code does not live in the main android thread
# So this code runs with a 1 MB default stack size. 
# This will force the use of the heap for the allocation of the scratchpad
if (ANDROID OR IOS)
  if( BUILD_GUI_DEPS )
    add_definitions(-DFORCE_USE_HEAP=1)
  endif()
endif()


