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

#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "BlockingQueue.h"
#include "ConsoleTools.h"

namespace Common {

class AsyncConsoleReader {

public:

  AsyncConsoleReader();
  ~AsyncConsoleReader();

  void start();
  bool getline(std::string& line);
  void stop();
  bool stopped() const;

private:

  void consoleThread();
  bool waitInput();

  std::atomic<bool> m_stop;
  std::thread m_thread;
  BlockingQueue<std::string> m_queue;
};


class ConsoleHandler {
public:

  ~ConsoleHandler();

  typedef std::function<bool(const std::vector<std::string> &)> ConsoleCommandHandler;

  std::string getUsage() const;
  void setHandler(const std::string& command, const ConsoleCommandHandler& handler, const std::string& usage = "");
  void requestStop();
  bool runCommand(const std::vector<std::string>& cmdAndArgs);

  void start(bool startThread = true, const std::string& prompt = "", Console::Color promptColor = Console::Color::Default);
  void stop();
  void wait(); 

private:

  typedef std::map<std::string, std::pair<ConsoleCommandHandler, std::string>> CommandHandlersMap;

  virtual void handleCommand(const std::string& cmd);

  void handlerThread();

  std::thread m_thread;
  std::string m_prompt;
  Console::Color m_promptColor = Console::Color::Default;
  CommandHandlersMap m_handlers;
  AsyncConsoleReader m_consoleReader;
};

}
