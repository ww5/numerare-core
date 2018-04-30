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

#include <algorithm>
#include <mutex>
#include <vector>

namespace Tools {

template<typename T>
class ObserverManager {
public:
  bool add(T* observer) {
    std::unique_lock<std::mutex> lock(m_observersMutex);
    auto it = std::find(m_observers.begin(), m_observers.end(), observer);
    if (m_observers.end() == it) {
      m_observers.push_back(observer);
      return true;
    } else {
      return false;
    }
  }

  bool remove(T* observer) {
    std::unique_lock<std::mutex> lock(m_observersMutex);

    auto it = std::find(m_observers.begin(), m_observers.end(), observer);
    if (m_observers.end() == it) {
      return false;
    } else {
      m_observers.erase(it);
      return true;
    }
  }

  void clear() {
    std::unique_lock<std::mutex> lock(m_observersMutex);
    m_observers.clear();
  }

#if defined(_MSC_VER)
  template<typename F>
  void notify(F notification) {
    std::vector<T*> observersCopy;
    {
      std::unique_lock<std::mutex> lock(m_observersMutex);
      observersCopy = m_observers;
    }

    for (T* observer : observersCopy) {
      (observer->*notification)();
    }
  }

  template<typename F, typename Arg0>
  void notify(F notification, const Arg0& arg0) {
    std::vector<T*> observersCopy;
    {
      std::unique_lock<std::mutex> lock(m_observersMutex);
      observersCopy = m_observers;
    }

    for (T* observer : observersCopy) {
      (observer->*notification)(arg0);
    }
  }

  template<typename F, typename Arg0, typename Arg1>
  void notify(F notification, const Arg0& arg0, const Arg1& arg1) {
    std::vector<T*> observersCopy;
    {
      std::unique_lock<std::mutex> lock(m_observersMutex);
      observersCopy = m_observers;
    }

    for (T* observer : observersCopy) {
      (observer->*notification)(arg0, arg1);
    }
  }

  template<typename F, typename Arg0, typename Arg1, typename Arg2>
  void notify(F notification, const Arg0& arg0, const Arg1& arg1, const Arg2& arg2) {
    std::vector<T*> observersCopy;
    {
      std::unique_lock<std::mutex> lock(m_observersMutex);
      observersCopy = m_observers;
    }

    for (T* observer : observersCopy) {
      (observer->*notification)(arg0, arg1, arg2);
    }
  }

  template<typename F, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  void notify(F notification, const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
    std::vector<T*> observersCopy;
    {
      std::unique_lock<std::mutex> lock(m_observersMutex);
      observersCopy = m_observers;
    }

    for (T* observer : observersCopy) {
      (observer->*notification)(arg0, arg1, arg2, arg3);
    }
  }

  template<typename F, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  void notify(F notification, const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4) {
    std::vector<T*> observersCopy;
    {
      std::unique_lock<std::mutex> lock(m_observersMutex);
      observersCopy = m_observers;
    }

    for (T* observer : observersCopy) {
      (observer->*notification)(arg0, arg1, arg2, arg3, arg4);
    }
  }

  template<typename F, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
  void notify(F notification, const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4, const Arg5& arg5) {
    std::vector<T*> observersCopy;
    {
      std::unique_lock<std::mutex> lock(m_observersMutex);
      observersCopy = m_observers;
    }

    for (T* observer : observersCopy) {
      (observer->*notification)(arg0, arg1, arg2, arg3, arg4, arg5);
    }
  }

#else

  template<typename F, typename... Args>
  void notify(F notification, Args... args) {
    std::vector<T*> observersCopy;
    {
      std::unique_lock<std::mutex> lock(m_observersMutex);
      observersCopy = m_observers;
    }

    for (T* observer : observersCopy) {
      (observer->*notification)(args...);
    }
  }
#endif

private:
  std::vector<T*> m_observers;
  std::mutex m_observersMutex;
};

}
