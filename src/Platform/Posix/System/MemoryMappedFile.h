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

#include <cstdint>
#include <string>
#include <system_error>

namespace System {

class MemoryMappedFile {
public:
  MemoryMappedFile();
  ~MemoryMappedFile();

  void create(const std::string& path, uint64_t size, bool overwrite, std::error_code& ec);
  void create(const std::string& path, uint64_t size, bool overwrite);
  void open(const std::string& path, std::error_code& ec);
  void open(const std::string& path);
  void close(std::error_code& ec);
  void close();

  const std::string& path() const;
  uint64_t size() const;
  const uint8_t* data() const;
  uint8_t* data();
  bool isOpened() const;

  void rename(const std::string& newPath, std::error_code& ec);
  void rename(const std::string& newPath);

  void flush(uint8_t* data, uint64_t size, std::error_code& ec);
  void flush(uint8_t* data, uint64_t size);

  void swap(MemoryMappedFile& other);

private:
  int m_file;
  std::string m_path;
  uint64_t m_size;
  uint8_t* m_data;
};

}
