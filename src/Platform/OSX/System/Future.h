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

#include <condition_variable>
#include <mutex>
#include <thread>

namespace System {

namespace Detail {

namespace { 

enum class State : unsigned {
  STARTED,
  COMPLETED,
  CONSUMED
};

}

// Simplest possible future implementation. The reason why this class even exist is because currenty std future has a
// memory corrupting bug on OSX. Spawn a new thread, execute procedure in it, get result, and wait thread to shut down.
// Actualy, this is what libstdc++'s std::future is doing.
template<class T> class Future {
public:
  // Create a new thread, and run `operation` in it.
  explicit Future(std::function<T()>&& operation) : procedure(std::move(operation)), state(State::STARTED), worker{[this] { asyncOp(); }} {
  }

  // Wait for async op to complete, then if thread wasn't detached, join it.
  ~Future() {
    wait();
    if (worker.joinable()) {
      worker.join();
    }
  }

  // Get result of async operation. UB if called more than once.
  T get() const {
    assert(state != State::CONSUMED);
    wait();
    state = State::CONSUMED;
    if (currentException != nullptr) {
      std::rethrow_exception(currentException);
    }

    return std::move(result);
  }

  // Wait for async operation to complete, if op is already completed, return immediately.
  void wait() const {
    std::unique_lock<std::mutex> guard(operationMutex);
    while (state == State::STARTED) {
      operationCondition.wait(guard);
    }
  }

  bool valid() const {
    std::unique_lock<std::mutex> guard(operationMutex);
    return state != State::CONSUMED;
  }

private:
  // This function is executed in a separate thread.
  void asyncOp() {
    try {
      assert(procedure != nullptr);
      result = procedure();
    } catch (...) {
      currentException = std::current_exception();
    }

    std::unique_lock<std::mutex> guard(operationMutex);
    state = State::COMPLETED;
    operationCondition.notify_one();
  }

  mutable T result;
  std::function<T()> procedure;
  std::exception_ptr currentException;
  mutable std::mutex operationMutex;
  mutable std::condition_variable operationCondition;
  mutable State state;
  std::thread worker;
};

template<> class Future<void> {
public:
  // Create a new thread, and run `operation` in it.
  explicit Future(std::function<void()>&& operation) : procedure(std::move(operation)), state(State::STARTED), worker{[this] { asyncOp(); }} {
  }

  // Wait for async op to complete, then if thread wasn't detached, join it.
  ~Future() {
    wait();
    if (worker.joinable()) {
      worker.join();
    }
  }

  // Get result of async operation. UB if called more than once.
  void get() const {
    assert(state != State::CONSUMED);
    wait();
    state = State::CONSUMED;
    if (currentException != nullptr) {
      std::rethrow_exception(currentException);
    }
  }

  // Wait for async operation to complete, if op is already completed, return immediately.
  void wait() const {
    std::unique_lock<std::mutex> guard(operationMutex);
    while (state == State::STARTED) {
      operationCondition.wait(guard);
    }
  }

  bool valid() const {
    std::unique_lock<std::mutex> guard(operationMutex);
    return state != State::CONSUMED;
  }

private:
  // This function is executed in a separate thread.
  void asyncOp() {
    try {
      assert(procedure != nullptr);
      procedure();
    } catch (...) {
      currentException = std::current_exception();
    }

    std::unique_lock<std::mutex> guard(operationMutex);
    state = State::COMPLETED;
    operationCondition.notify_one();
  }

  std::function<void()> procedure;
  std::exception_ptr currentException;
  mutable std::mutex operationMutex;
  mutable std::condition_variable operationCondition;
  mutable State state;
  std::thread worker;
};

template<class T> std::function<T()> async(std::function<T()>&& operation) {
  return operation;
}

}

}
