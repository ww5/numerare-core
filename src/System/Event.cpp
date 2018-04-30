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

#include "Event.h"
#include <cassert>
#include <System/Dispatcher.h>
#include <System/InterruptedException.h>

namespace System {

namespace {

struct EventWaiter {
  bool interrupted;
  EventWaiter* prev;
  EventWaiter* next;
  NativeContext* context;
};

}

Event::Event() : dispatcher(nullptr) {
}

Event::Event(Dispatcher& dispatcher) : dispatcher(&dispatcher), state(false), first(nullptr) {
}

Event::Event(Event&& other) : dispatcher(other.dispatcher) {
  if (dispatcher != nullptr) {
    state = other.state;
    if (!state) {
      assert(other.first == nullptr);
      first = nullptr;
    }

    other.dispatcher = nullptr;
  }
}

Event::~Event() {
  assert(dispatcher == nullptr || state || first == nullptr);
}

Event& Event::operator=(Event&& other) {
  assert(dispatcher == nullptr || state || first == nullptr);
  dispatcher = other.dispatcher;
  if (dispatcher != nullptr) {
    state = other.state;
    if (!state) {
      assert(other.first == nullptr);
      first = nullptr;
    }

    other.dispatcher = nullptr;
  }

  return *this;
}

bool Event::get() const {
  assert(dispatcher != nullptr);
  return state;
}

void Event::clear() {
  assert(dispatcher != nullptr);
  if (state) {
    state = false;
    first = nullptr;
  }
}

void Event::set() {
  assert(dispatcher != nullptr);
  if (!state) {
    state = true;
    for (EventWaiter* waiter = static_cast<EventWaiter*>(first); waiter != nullptr; waiter = waiter->next) {
      waiter->context->interruptProcedure = nullptr;
      dispatcher->pushContext(waiter->context);
    }
  }
}

void Event::wait() {
  assert(dispatcher != nullptr);
  if (dispatcher->interrupted()) {
    throw InterruptedException();
  }

  if (!state) {
    EventWaiter waiter = { false, nullptr, nullptr, dispatcher->getCurrentContext() };
    waiter.context->interruptProcedure = [&] {
      if (waiter.next != nullptr) {
        assert(waiter.next->prev == &waiter);
        waiter.next->prev = waiter.prev;
      } else {
        assert(last == &waiter);
        last = waiter.prev;
      }

      if (waiter.prev != nullptr) { 
        assert(waiter.prev->next == &waiter);
        waiter.prev->next = waiter.next;
      } else {
        assert(first == &waiter);
        first = waiter.next;
      }

      assert(!waiter.interrupted);
      waiter.interrupted = true;
      dispatcher->pushContext(waiter.context);
    };

    if (first != nullptr) {
      static_cast<EventWaiter*>(last)->next = &waiter;
      waiter.prev = static_cast<EventWaiter*>(last);
    } else {
      first = &waiter;
    }

    last = &waiter;
    dispatcher->dispatch();
    assert(waiter.context == dispatcher->getCurrentContext());
    assert( waiter.context->interruptProcedure == nullptr);
    assert(dispatcher != nullptr);
    if (waiter.interrupted) {
      throw InterruptedException();
    } 
  }
}

}
