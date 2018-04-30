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

#include <string>
#include <cstdint>

#include <Common/StringView.h>

namespace CryptoNote {

class ISerializer {
public:

  enum SerializerType {
    INPUT,
    OUTPUT
  };

  virtual ~ISerializer() {}

  virtual SerializerType type() const = 0;

  virtual bool beginObject(Common::StringView name) = 0;
  virtual void endObject() = 0;
  virtual bool beginArray(size_t& size, Common::StringView name) = 0;
  virtual void endArray() = 0;

  virtual bool operator()(uint8_t& value, Common::StringView name) = 0;
  virtual bool operator()(int16_t& value, Common::StringView name) = 0;
  virtual bool operator()(uint16_t& value, Common::StringView name) = 0;
  virtual bool operator()(int32_t& value, Common::StringView name) = 0;
  virtual bool operator()(uint32_t& value, Common::StringView name) = 0;
  virtual bool operator()(int64_t& value, Common::StringView name) = 0;
  virtual bool operator()(uint64_t& value, Common::StringView name) = 0;
  virtual bool operator()(double& value, Common::StringView name) = 0;
  virtual bool operator()(bool& value, Common::StringView name) = 0;
  virtual bool operator()(std::string& value, Common::StringView name) = 0;
  
  // read/write binary block
  virtual bool binary(void* value, size_t size, Common::StringView name) = 0;
  virtual bool binary(std::string& value, Common::StringView name) = 0;

  template<typename T>
  bool operator()(T& value, Common::StringView name);
};

template<typename T>
bool ISerializer::operator()(T& value, Common::StringView name) {
  return serialize(value, name, *this);
}

template<typename T>
bool serialize(T& value, Common::StringView name, ISerializer& serializer) {
  if (!serializer.beginObject(name)) {
    return false;
  }

  serialize(value, serializer);
  serializer.endObject();
  return true;
}

template<typename T>
void serialize(T& value, ISerializer& serializer) {
  value.serialize(serializer);
}

#ifdef __clang__
template<> inline
bool ISerializer::operator()(size_t& value, Common::StringView name) {
  return operator()(*reinterpret_cast<uint64_t*>(&value), name);
}
#endif

#define KV_MEMBER(member) s(member, #member);

}
