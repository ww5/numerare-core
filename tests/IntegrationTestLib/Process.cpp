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

#include "Process.h"

#include <cstdlib>
#include <sstream>
#include <stdexcept>

#ifndef _WIN32
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#endif

namespace Tests {

  void Process::startChild(const std::string& executablePath, const std::vector<std::string>& args) {
   
#if defined WIN32
    std::stringstream ss;
    ss << "start /MIN " << executablePath;

    for (const auto& arg: args) {
      ss << " \"" << arg << "\"";
    }

    auto cmdline = ss.str();
    system(cmdline.c_str());

#else
    std::vector<const char*> cargs;
    cargs.push_back(executablePath.c_str());
    for (const auto& arg : args) {
      cargs.push_back(arg.c_str());
    }

    cargs.push_back(nullptr);

    auto pid = fork();

    if (pid == 0) {
      if (execv(executablePath.c_str(), (char**)&cargs[0]) == -1) {
        printf("Failed to start %s: %d\n", executablePath.c_str(), errno);
        exit(404);
      }
    } else if (pid > 0) {
      m_pid = pid;
    } else if (pid < 0) {
      throw std::runtime_error("fork() failed");
    }
#endif

  }

  void Process::wait() {
#ifndef _WIN32
    if (m_pid == 0) {
      return;
    }

    int status;
    waitpid(m_pid, &status, 0);
    m_pid = 0;
#endif
  }

}
