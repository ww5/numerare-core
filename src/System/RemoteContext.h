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

#include <cassert>
#include <System/Dispatcher.h>
#include <System/Event.h>
#include <System/Future.h>
#include <System/InterruptedException.h>

namespace System {

template<class T = void> class RemoteContext {
public:
  // Start a thread, execute operation in it, continue execution of current context.
  RemoteContext(Dispatcher& d, std::function<T()>&& operation)
      : dispatcher(d), event(d), procedure(std::move(operation)), future(System::Detail::async<T>([this] { return asyncProcedure(); })), interrupted(false) {
  }

  // Run other task on dispatcher until future is ready, then return lambda's result, or rethrow exception. UB if called more than once.
  T get() const {
    wait();
    return future.get();
  }

  // Run other task on dispatcher until future is ready.
  void wait() const {
    while (!event.get()) {
      try {
        event.wait();
      } catch (InterruptedException&) {
        interrupted = true;
      }
    }

    if (interrupted) {
      dispatcher.interrupt();
    }
  }

  // Wait future to complete.
  ~RemoteContext() {
    try {
      wait();
    } catch (...) {
    }

    try {
      // windows future implementation doesn't wait for completion on destruction
      if (future.valid()) {
        future.wait();
      }
    } catch (...) {
    }
  }

private:
  struct NotifyOnDestruction {
    NotifyOnDestruction(Dispatcher& d, Event& e) : dispatcher(d), event(e) {
    }

    ~NotifyOnDestruction() {
      // make a local copy; event reference will be dead when function is called
      auto localEvent = &event;
      // die if this throws...
      dispatcher.remoteSpawn([=] { localEvent->set(); });
    }

    Dispatcher& dispatcher;
    Event& event;
  };

  // This function is executed in future object
  T asyncProcedure() {
    NotifyOnDestruction guard(dispatcher, event);
    assert(procedure != nullptr);
    return procedure();
  }

  Dispatcher& dispatcher;
  mutable Event event;
  std::function<T()> procedure;
  mutable System::Detail::Future<T> future;
  mutable bool interrupted;
};

}
