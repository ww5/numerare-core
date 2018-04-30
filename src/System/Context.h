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

#include <System/Dispatcher.h>
#include <System/Event.h>
#include <System/InterruptedException.h>

namespace System {

template<typename ResultType = void>
class Context {
public:
  Context(Dispatcher& dispatcher, std::function<ResultType()>&& target) : 
    dispatcher(dispatcher), target(std::move(target)), ready(dispatcher), bindingContext(dispatcher.getReusableContext()) {
    bindingContext.interrupted = false;
    bindingContext.groupNext = nullptr;
    bindingContext.groupPrev = nullptr;
    bindingContext.group = nullptr;
    bindingContext.procedure = [this] {
      try {
        new(resultStorage) ResultType(this->target());
      } catch (...) {
        exceptionPointer = std::current_exception();
      }

      ready.set();
    };

    dispatcher.pushContext(&bindingContext);
  }

  Context(const Context&) = delete;  
  Context& operator=(const Context&) = delete;

  ~Context() {
    interrupt();
    wait();
    dispatcher.pushReusableContext(bindingContext);
  }

  ResultType& get() {
    wait();
    if (exceptionPointer != nullptr) {
      std::rethrow_exception(exceptionPointer);
    }

    return *reinterpret_cast<ResultType*>(resultStorage);
  }

  void interrupt() {
    dispatcher.interrupt(&bindingContext);
  }

  void wait() {
    for (;;) {
      try {
        ready.wait();
        break;
      } catch (InterruptedException&) {
        interrupt();
      }
    }
  }

private:
  uint8_t resultStorage[sizeof(ResultType)];
  Dispatcher& dispatcher;
  std::function<ResultType()> target;
  Event ready;
  NativeContext& bindingContext;
  std::exception_ptr exceptionPointer;
};

template<>
class Context<void> {
public:
  Context(Dispatcher& dispatcher, std::function<void()>&& target) :
    dispatcher(dispatcher), target(std::move(target)), ready(dispatcher), bindingContext(dispatcher.getReusableContext()) {
    bindingContext.interrupted = false;
    bindingContext.groupNext = nullptr;
    bindingContext.groupPrev = nullptr;
    bindingContext.group = nullptr;
    bindingContext.procedure = [this] {
      try {
        this->target();
      } catch (...) {
        exceptionPointer = std::current_exception();
      }

      ready.set();
    };

    dispatcher.pushContext(&bindingContext);
  }

  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;

  ~Context() {
    interrupt();
    wait();
    dispatcher.pushReusableContext(bindingContext);
  }

  void get() {
    wait();
    if (exceptionPointer != nullptr) {
      std::rethrow_exception(exceptionPointer);
    }
  }

  void interrupt() {
    dispatcher.interrupt(&bindingContext);
  }

  void wait() {
    for (;;) {
      try {
        ready.wait();
        break;
      } catch (InterruptedException&) {
        interrupt();
      }
    }
  }

private:
  Dispatcher& dispatcher;
  std::function<void()> target;
  Event ready;
  NativeContext& bindingContext;
  std::exception_ptr exceptionPointer;
};

}
