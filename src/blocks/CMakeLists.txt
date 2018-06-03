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
    add_library(blocks STATIC blockexports.c)
    set_target_properties(blocks PROPERTIES LINKER_LANGUAGE C)
else()
    add_custom_command(OUTPUT blocks.o MAIN_DEPENDENCY blocks.dat COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && ${CMAKE_LINKER} ${LD_RAW_FLAGS} -r -b binary -o ${CMAKE_CURRENT_BINARY_DIR}/blocks.o blocks.dat)
    add_custom_command(OUTPUT testnet_blocks.o MAIN_DEPENDENCY testnet_blocks.dat COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && ${CMAKE_LINKER} ${LD_RAW_FLAGS} -r -b binary -o ${CMAKE_CURRENT_BINARY_DIR}/testnet_blocks.o testnet_blocks.dat)
    add_custom_command(OUTPUT stagenet_blocks.o MAIN_DEPENDENCY stagenet_blocks.dat COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && ${CMAKE_LINKER} ${LD_RAW_FLAGS} -r -b binary -o ${CMAKE_CURRENT_BINARY_DIR}/stagenet_blocks.o stagenet_blocks.dat)
    add_library(blocks STATIC blocks.o testnet_blocks.o stagenet_blocks.o blockexports.c)
    set_target_properties(blocks PROPERTIES LINKER_LANGUAGE C)
endif()

