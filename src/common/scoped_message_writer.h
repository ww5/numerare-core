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

#include "misc_log_ex.h"
#include <iostream>

#ifdef HAVE_READLINE
  #include "readline_buffer.h"
  #define PAUSE_READLINE() \
    rdln::suspend_readline pause_readline; 
#else
  #define PAUSE_READLINE()
#endif

namespace tools
{

/************************************************************************/
/*                                                                      */
/************************************************************************/
class scoped_message_writer
{
private:
  bool m_flush;
  std::stringstream m_oss;
  epee::console_colors m_color;
  bool m_bright;
  el::Level m_log_level;
public:
  scoped_message_writer(
      epee::console_colors color = epee::console_color_default
    , bool bright = false
    , std::string&& prefix = std::string()
    , el::Level log_level = el::Level::Info
    )
    : m_flush(true)
    , m_color(color)
    , m_bright(bright)
    , m_log_level(log_level)
  {
    m_oss << prefix;
  }

  scoped_message_writer(scoped_message_writer&& rhs)
    : m_flush(std::move(rhs.m_flush))
#if defined(_MSC_VER)
    , m_oss(std::move(rhs.m_oss))
#else
      // GCC bug: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=54316
    , m_oss(rhs.m_oss.str(), std::ios_base::out | std::ios_base::ate)
#endif
    , m_color(std::move(rhs.m_color))
    , m_log_level(std::move(rhs.m_log_level))
  {
    rhs.m_flush = false;
  }

  scoped_message_writer(scoped_message_writer& rhs) = delete;
  scoped_message_writer& operator=(scoped_message_writer& rhs) = delete;
  scoped_message_writer& operator=(scoped_message_writer&& rhs) = delete;

  template<typename T>
  std::ostream& operator<<(const T& val)
  {
    m_oss << val;
    return m_oss;
  }

  ~scoped_message_writer()
  {
    if (m_flush)
    {
      m_flush = false;

      MCLOG_FILE(m_log_level, "msgwriter", m_oss.str());

      if (epee::console_color_default == m_color)
      {
        std::cout << m_oss.str();
      }
      else
      {
        PAUSE_READLINE();
        set_console_color(m_color, m_bright);
        std::cout << m_oss.str();
        epee::reset_console_color();
      }
      std::cout << std::endl;
    }
  }
};

inline scoped_message_writer success_msg_writer(bool color = true)
{
  return scoped_message_writer(color ? epee::console_color_green : epee::console_color_default, false, std::string(), el::Level::Info);
}

inline scoped_message_writer msg_writer(epee::console_colors color = epee::console_color_default)
{
  return scoped_message_writer(color, false, std::string(), el::Level::Info);
}

inline scoped_message_writer fail_msg_writer()
{
  return scoped_message_writer(epee::console_color_red, true, "Error: ", el::Level::Error);
}

} // namespace tools
