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

#include "gtest/gtest.h"
#include "Common/PathTools.h"

TEST(PathTools, NativePathToGeneric) {

#ifdef _WIN32
  const std::string input = "C:\\Windows\\System\\etc\\file.exe";
  const std::string output = "C:/Windows/System/etc/file.exe";
#else
  const std::string input = "/var/tmp/file.tmp";
  const std::string output = input;

#endif

  auto path = Common::NativePathToGeneric(input);
  ASSERT_EQ(output, path);
}

TEST(PathTools, GetExtension) {
  ASSERT_EQ("", Common::GetExtension(""));
  ASSERT_EQ(".ext", Common::GetExtension(".ext"));

  ASSERT_EQ("", Common::GetExtension("test"));
  ASSERT_EQ(".ext", Common::GetExtension("test.ext"));
  ASSERT_EQ(".ext2", Common::GetExtension("test.ext.ext2"));

  ASSERT_EQ(".ext", Common::GetExtension("/path/file.ext"));
  ASSERT_EQ(".yyy", Common::GetExtension("/path.xxx/file.yyy"));
  ASSERT_EQ("", Common::GetExtension("/path.ext/file"));
}

TEST(PathTools, RemoveExtension) {

  ASSERT_EQ("", Common::RemoveExtension(""));
  ASSERT_EQ("", Common::RemoveExtension(".ext"));

  ASSERT_EQ("test", Common::RemoveExtension("test"));
  ASSERT_EQ("test", Common::RemoveExtension("test.ext"));
  ASSERT_EQ("test.ext", Common::RemoveExtension("test.ext.ext2"));

  ASSERT_EQ("/path/file", Common::RemoveExtension("/path/file.ext"));
  ASSERT_EQ("/path.ext/file", Common::RemoveExtension("/path.ext/file.ext"));
  ASSERT_EQ("/path.ext/file", Common::RemoveExtension("/path.ext/file"));
}

TEST(PathTools, SplitPath) {
  std::string dir;
  std::string file;

  Common::SplitPath("/path/more/file", dir, file);

  ASSERT_EQ("/path/more", dir);
  ASSERT_EQ("file", file);

  Common::SplitPath("file.ext", dir, file);

  ASSERT_EQ("", dir);
  ASSERT_EQ("file.ext", file);

  Common::SplitPath("/path/more/", dir, file);

  ASSERT_EQ("/path/more", dir);
  ASSERT_EQ("", file);
}
