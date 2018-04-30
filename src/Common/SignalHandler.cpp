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

#include "SignalHandler.h"

#include <mutex>
#include <iostream>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#else
#include <signal.h>
#include <cstring>
#endif

namespace {

  std::function<void(void)> m_handler;

  void handleSignal() {
    static std::mutex m_mutex;
    std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);
    if (!lock.owns_lock()) {
      return;
    }
    m_handler();
  }


#if defined(WIN32)
BOOL WINAPI winHandler(DWORD type) {
  if (CTRL_C_EVENT == type || CTRL_BREAK_EVENT == type) {
    handleSignal();
    return TRUE;
  } else {
    std::cerr << "Got control signal " << type << ". Exiting without saving...";
    return FALSE;
  }
  return TRUE;
}

#else

void posixHandler(int /*type*/) {
  handleSignal();
}
#endif

}


namespace Tools {

  bool SignalHandler::install(std::function<void(void)> t)
  {
#if defined(WIN32)
    bool r = TRUE == ::SetConsoleCtrlHandler(&winHandler, TRUE);
    if (r)  {
      m_handler = t;
    }
    return r;
#else
    struct sigaction newMask;
    std::memset(&newMask, 0, sizeof(struct sigaction));
    newMask.sa_handler = posixHandler;
    if (sigaction(SIGINT, &newMask, nullptr) != 0) {
      return false;
    }

    if (sigaction(SIGTERM, &newMask, nullptr) != 0) {
      return false;
    }

    std::memset(&newMask, 0, sizeof(struct sigaction));
    newMask.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &newMask, nullptr) != 0) {
      return false;
    }

    m_handler = t;
    return true;
#endif
  }

}
