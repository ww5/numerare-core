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

if(MSVC OR MINGW)
  set(daemonizer_sources
    windows_service.cpp
    windows_daemonizer.inl
  )
else()
  set(daemonizer_sources
    posix_fork.cpp
    posix_daemonizer.inl
  )
endif()

set(daemonizer_headers
)

if(MSVC OR MINGW)
  set(daemonizer_private_headers
    daemonizer.h
    windows_service.h
    windows_service_runner.h
  )
else()
  set(daemonizer_private_headers
    daemonizer.h
    posix_fork.h
  )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG_TMPDIR_LOG=1)
endif()

monero_private_headers(daemonizer
  ${daemonizer_private_headers})
monero_add_library(daemonizer
  ${daemonizer_sources}
  ${daemonizer_headers}
  ${daemonizer_private_headers})
target_link_libraries(daemonizer
  PUBLIC
    common
    ${Boost_CHRONO_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
  PRIVATE
    ${EXTRA_LIBRARIES})
