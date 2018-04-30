/***
	MIT License

	Copyright (c) 2018 NUMERARE

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	Parts of this file are originally Copyright (c) 2012-2017 The CryptoNote developers, The Bytecoin developers
***/

#include "ConsoleTools.h"

#include <stdio.h>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <io.h>
#else
#include <iostream>
#include <unistd.h>
#endif

namespace Common { namespace Console { 

bool isConsoleTty() {
#if defined(WIN32)
  static bool istty = 0 != _isatty(_fileno(stdout));
#else
  static bool istty = 0 != isatty(fileno(stdout));
#endif
  return istty;
}

void setTextColor(Color color) {
  if (!isConsoleTty()) {
    return;
  }

  if (color > Color::BrightMagenta) {
    color = Color::Default;
  }

#ifdef _WIN32

  static WORD winColors[] = {
    // default
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    // main
    FOREGROUND_BLUE,
    FOREGROUND_GREEN,
    FOREGROUND_RED,
    FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_BLUE,
    // bright
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
  };

  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), winColors[static_cast<size_t>(color)]);

#else

  static const char* ansiColors[] = {
    // default
    "\033[0m",
    // main
    "\033[0;34m",
    "\033[0;32m",
    "\033[0;31m",
    "\033[0;33m",
    "\033[0;37m",
    "\033[0;36m",
    "\033[0;35m",
    // bright
    "\033[1;34m",
    "\033[1;32m",
    "\033[1;31m",
    "\033[1;33m",
    "\033[1;37m",
    "\033[1;36m",
    "\033[1;35m"
  };

  std::cout << ansiColors[static_cast<size_t>(color)];

#endif

}

}}
