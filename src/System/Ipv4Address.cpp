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

#include "Ipv4Address.h"
#include <stdexcept>

namespace System {

namespace {

uint8_t readUint8(const std::string& source, size_t& offset) {
  if (offset == source.size() || source[offset] < '0' || source[offset] > '9') {
    throw std::runtime_error("Unable to read value from string");
  }

  uint8_t value = source[offset] - '0';
  if (offset + 1 == source.size() || source[offset + 1] < '0' || source[offset + 1] > '9') {
    offset = offset + 1;
    return value;
  }

  if (value == 0) {
    throw std::runtime_error("Unable to read value from string");
  }

  value = value * 10 + (source[offset + 1] - '0');
  if (offset + 2 == source.size() || source[offset + 2] < '0' || source[offset + 2] > '9') {
    offset = offset + 2;
    return value;
  }

  if ((value == 25 && source[offset + 2] > '5') || value > 25) {
    throw std::runtime_error("Unable to read value from string");
  }

  value = value * 10 + (source[offset + 2] - '0');
  offset = offset + 3;
  return value;
}

}

Ipv4Address::Ipv4Address(uint32_t value) : value(value) {
}

Ipv4Address::Ipv4Address(const std::string& dottedDecimal) {
  size_t offset = 0;
  value = readUint8(dottedDecimal, offset);
  if (offset == dottedDecimal.size() || dottedDecimal[offset] != '.') {
    throw std::runtime_error("Invalid Ipv4 address string");
  }

  ++offset;
  value = value << 8 | readUint8(dottedDecimal, offset);
  if (offset == dottedDecimal.size() || dottedDecimal[offset] != '.') {
    throw std::runtime_error("Invalid Ipv4 address string");
  }

  ++offset;
  value = value << 8 | readUint8(dottedDecimal, offset);
  if (offset == dottedDecimal.size() || dottedDecimal[offset] != '.') {
    throw std::runtime_error("Invalid Ipv4 address string");
  }

  ++offset;
  value = value << 8 | readUint8(dottedDecimal, offset);
  if (offset < dottedDecimal.size()) {
    throw std::runtime_error("Invalid Ipv4 address string");
  }
}

bool Ipv4Address::operator!=(const Ipv4Address& other) const {
  return value != other.value;
}

bool Ipv4Address::operator==(const Ipv4Address& other) const {
  return value == other.value;
}

uint32_t Ipv4Address::getValue() const {
  return value;
}

std::string Ipv4Address::toDottedDecimal() const {
  std::string result;
  result += std::to_string(value >> 24);
  result += '.';
  result += std::to_string(value >> 16 & 255);
  result += '.';
  result += std::to_string(value >> 8 & 255);
  result += '.';
  result += std::to_string(value & 255);
  return result;
}

bool Ipv4Address::isLoopback() const {
  // 127.0.0.0/8
  return (value & 0xff000000) == (127 << 24);
}

bool Ipv4Address::isPrivate() const {
  return
    // 10.0.0.0/8
    (value & 0xff000000) == (10 << 24) ||
    // 172.16.0.0/12
    (value & 0xfff00000) == ((172 << 24) | (16 << 16)) ||
    // 192.168.0.0/16
    (value & 0xffff0000) == ((192 << 24) | (168 << 16));
}

}
