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

namespace CryptoNote {

//Value must have public method IntrusiveLinkedList<Value>::hook& getHook()
template<class Value> class IntrusiveLinkedList {
public:
  class hook {
  public:
    friend class IntrusiveLinkedList<Value>;

    hook();
  private:
    Value* prev;
    Value* next;
    bool used;
  };

  class iterator : public std::iterator<std::bidirectional_iterator_tag, Value> {
  public:
    iterator(Value* value);

    bool operator!=(const iterator& other) const;
    bool operator==(const iterator& other) const;
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);

    Value& operator*() const;
    Value* operator->() const;

  private:
    Value* currentElement;
  };

  IntrusiveLinkedList();

  bool insert(Value& value);
  bool remove(Value& value);

  bool empty() const;

  iterator begin();
  iterator end();
private:
  Value* head;
  Value* tail;
};

template<class Value>
IntrusiveLinkedList<Value>::IntrusiveLinkedList() : head(nullptr), tail(nullptr) {}

template<class Value>
bool IntrusiveLinkedList<Value>::insert(Value& value) {
  if (!value.getHook().used) {
    if (head == nullptr) {
      head = &value;
      tail = head;
      value.getHook().prev = nullptr;
    } else {
      tail->getHook().next = &value;
      value.getHook().prev = tail;
      tail = &value;
    }

    value.getHook().next = nullptr;
    value.getHook().used = true;
    return true;
  } else {
    return false;
  }
}

template<class Value>
bool IntrusiveLinkedList<Value>::remove(Value& value) {
  if (value.getHook().used && head != nullptr) {
    Value* toRemove = &value;
    Value* current = head;
    while (current->getHook().next != nullptr) {
      if (toRemove == current) {
        break;
      }

      current = current->getHook().next;
    }

    if (toRemove == current) {
      if (current->getHook().prev == nullptr) {
        assert(current == head);
        head = current->getHook().next;

        if (head != nullptr) {
          head->getHook().prev = nullptr;
        } else {
          tail = nullptr;
        }
      } else {
        current->getHook().prev->getHook().next = current->getHook().next;
        if (current->getHook().next != nullptr) {
          current->getHook().next->getHook().prev = current->getHook().prev;
        } else {
          tail = current->getHook().prev;
        }
      }

      current->getHook().prev = nullptr;
      current->getHook().next = nullptr;
      current->getHook().used = false;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

template<class Value>
bool IntrusiveLinkedList<Value>::empty() const {
  return head == nullptr;
}

template<class Value>
typename IntrusiveLinkedList<Value>::iterator IntrusiveLinkedList<Value>::begin() {
  return iterator(head);
}

template<class Value>
typename IntrusiveLinkedList<Value>::iterator IntrusiveLinkedList<Value>::end() {
  return iterator(nullptr);
}

template<class Value>
IntrusiveLinkedList<Value>::hook::hook() : prev(nullptr), next(nullptr), used(false) {}

template<class Value>
IntrusiveLinkedList<Value>::iterator::iterator(Value* value) : currentElement(value) {}

template<class Value>
bool IntrusiveLinkedList<Value>::iterator::operator!=(const typename IntrusiveLinkedList<Value>::iterator& other) const {
  return currentElement != other.currentElement;
}

template<class Value>
bool IntrusiveLinkedList<Value>::iterator::operator==(const typename IntrusiveLinkedList<Value>::iterator& other) const {
  return currentElement == other.currentElement;
}

template<class Value>
typename IntrusiveLinkedList<Value>::iterator& IntrusiveLinkedList<Value>::iterator::operator++() {
  assert(currentElement != nullptr);
  currentElement = currentElement->getHook().next;
  return *this;
}

template<class Value>
typename IntrusiveLinkedList<Value>::iterator IntrusiveLinkedList<Value>::iterator::operator++(int) {
  IntrusiveLinkedList<Value>::iterator copy = *this;

  assert(currentElement != nullptr);
  currentElement = currentElement->getHook().next;
  return copy;
}

template<class Value>
typename IntrusiveLinkedList<Value>::iterator& IntrusiveLinkedList<Value>::iterator::operator--() {
  assert(currentElement != nullptr);
  currentElement = currentElement->getHook().prev;
  return *this;
}

template<class Value>
typename IntrusiveLinkedList<Value>::iterator IntrusiveLinkedList<Value>::iterator::operator--(int) {
  IntrusiveLinkedList<Value>::iterator copy = *this;

  assert(currentElement != nullptr);
  currentElement = currentElement->getHook().prev;
  return copy;
}

template<class Value>
Value& IntrusiveLinkedList<Value>::iterator::operator*() const {
  assert(currentElement != nullptr);

  return *currentElement; 
}

template<class Value>
Value* IntrusiveLinkedList<Value>::iterator::operator->() const {
  return currentElement; 
}

}
