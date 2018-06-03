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

#include <string>
#include <atomic>
#include <boost/optional/optional.hpp>
#include "wipeable_string.h"

namespace tools
{
  class password_container
  {
  public:
    static constexpr const size_t max_password_size = 1024;

    //! Empty password
    password_container() noexcept;

    //! `password` is used as password
    password_container(std::string&& password) noexcept;

    //! \return A password from stdin TTY prompt or `std::cin` pipe.
    static boost::optional<password_container> prompt(bool verify, const char *mesage = "Password");
    static std::atomic<bool> is_prompting;

    password_container(const password_container&) = delete;
    password_container(password_container&& rhs) = default;

    //! Wipes internal password
    ~password_container() noexcept;

    password_container& operator=(const password_container&) = delete;
    password_container& operator=(password_container&&) = default;

    const epee::wipeable_string &password() const noexcept { return m_password; }

  private:
    epee::wipeable_string m_password;
  };

  struct login
  {
    login() = default;

    /*!
       Extracts username and password from the format `username:password`. A
       blank username or password is allowed. If the `:` character is not
       present, `password_container::prompt` will be called by forwarding the
       `verify` and `message` arguments.

       \param userpass Is "consumed", and the memory contents are wiped.
       \param verify is passed to `password_container::prompt` if necessary.
       \param message is passed to `password_container::prompt` if necessary.

       \return The username and password, or boost::none if
         `password_container::prompt` fails.
     */
    static boost::optional<login> parse(std::string&& userpass, bool verify, const std::function<boost::optional<password_container>(bool)> &prompt);

    login(const login&) = delete;
    login(login&&) = default;
    ~login() = default;
    login& operator=(const login&) = delete;
    login& operator=(login&&) = default;

    std::string username;
    password_container password;
  };
}
