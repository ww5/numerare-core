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

#include <queue>

#include "IntrusiveLinkedList.h"

#include "System/Dispatcher.h"
#include "System/Event.h"
#include "System/InterruptedException.h"

namespace CryptoNote {

template <class MessageType> class MessageQueue {
public:
  MessageQueue(System::Dispatcher& dispatcher);

  const MessageType& front();
  void pop();
  void push(const MessageType& message);

  void stop();

private:
  friend class IntrusiveLinkedList<MessageQueue<MessageType>>;
  typename IntrusiveLinkedList<MessageQueue<MessageType>>::hook& getHook();
  void wait();

  System::Dispatcher& dispatcher;
  std::queue<MessageType> messageQueue;
  System::Event event;
  bool stopped;

  typename IntrusiveLinkedList<MessageQueue<MessageType>>::hook hook;
};

template <class MessageQueueContainer, class MessageType> class MesageQueueGuard {
public:
  MesageQueueGuard(MessageQueueContainer& container, MessageQueue<MessageType>& messageQueue)
      : container(container), messageQueue(messageQueue) {
    container.addMessageQueue(messageQueue);
  }

  ~MesageQueueGuard() {
    container.removeMessageQueue(messageQueue);
  }

private:
  MessageQueueContainer& container;
  MessageQueue<MessageType>& messageQueue;
};

template <class MessageType>
MessageQueue<MessageType>::MessageQueue(System::Dispatcher& dispatch)
    : dispatcher(dispatch), event(dispatch), stopped(false) {
}

template <class MessageType> void MessageQueue<MessageType>::wait() {
  if (messageQueue.empty()) {
    if (stopped) {
      throw System::InterruptedException();
    }

    event.clear();
    while (!event.get()) {
      event.wait();

      if (stopped) {
        throw System::InterruptedException();
      }
    }
  }
}

template <class MessageType> const MessageType& MessageQueue<MessageType>::front() {
  wait();
  return messageQueue.front();
}

template <class MessageType> void MessageQueue<MessageType>::pop() {
  wait();
  messageQueue.pop();
}

template <class MessageType> void MessageQueue<MessageType>::push(const MessageType& message) {
  dispatcher.remoteSpawn([=]() mutable {
    messageQueue.push(std::move(message));
    event.set();
  });
}

template <class MessageType> void MessageQueue<MessageType>::stop() {
  stopped = true;
  event.set();
}

template <class MessageType>
typename IntrusiveLinkedList<MessageQueue<MessageType>>::hook& MessageQueue<MessageType>::getHook() {
  return hook;
}
}
