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

	Parts of this file are originally Copyright (c) 2012-2014 The CryptoNote developers,
	                                                2014-2018 The Monero Project
***/

#pragma once

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>
#include <stdexcept>

namespace tools
{
//! A global thread pool
class threadpool
{
public:
  static threadpool& getInstance() {
    static threadpool instance;
    return instance;
  }

  // The waiter lets the caller know when all of its
  // tasks are completed.
  class waiter {
    boost::mutex mt;
    boost::condition_variable cv;
    int num;
    public:
    void inc();
    void dec();
    void wait();  //! Wait for a set of tasks to finish.
    waiter() : num(0){}
    ~waiter();
  };

  // Submit a task to the pool. The waiter pointer may be
  // NULL if the caller doesn't care to wait for the
  // task to finish.
  void submit(waiter *waiter, std::function<void()> f);

  int get_max_concurrency();

  private:
    threadpool();
    ~threadpool();
    typedef struct entry {
      waiter *wo;
      std::function<void()> f;
    } entry;
    std::deque<entry> queue;
    boost::condition_variable has_work;
    boost::mutex mutex;
    std::vector<boost::thread> threads;
    int active;
    int max;
    bool running;
    void run();
};

}
