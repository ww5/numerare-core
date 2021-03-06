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

#if !defined __GNUC__ || defined __MINGW32__ || defined __MINGW64__ || defined __ANDROID__
#define USE_UNWIND
#else
#define ELPP_FEATURE_CRASH_LOG 1
#endif
#include "easylogging++/easylogging++.h"

#include <stdexcept>
#ifdef USE_UNWIND
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <cxxabi.h>
#endif
#ifndef STATICLIB
#include <dlfcn.h>
#endif
#include <boost/algorithm/string.hpp>
#include "common/stack_trace.h"
#include "misc_log_ex.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "stacktrace"

#define ST_LOG(x) CINFO(el::base::Writer,el::base::DispatchAction::FileOnlyLog,MONERO_DEFAULT_LOG_CATEGORY) << x

// from http://stackoverflow.com/questions/11665829/how-can-i-print-stack-trace-for-caught-exceptions-in-c-code-injection-in-c

// The decl of __cxa_throw in /usr/include/.../cxxabi.h uses
// 'std::type_info *', but GCC's built-in protype uses 'void *'.
#ifdef __clang__
#define CXA_THROW_INFO_T std::type_info
#else // !__clang__
#define CXA_THROW_INFO_T void
#endif // !__clang__

#ifdef STATICLIB
#define CXA_THROW __wrap___cxa_throw
extern "C"
__attribute__((noreturn))
void __real___cxa_throw(void *ex, CXA_THROW_INFO_T *info, void (*dest)(void*));
#else // !STATICLIB
#define CXA_THROW __cxa_throw
extern "C"
typedef
#ifdef __clang__ // only clang, not GCC, lets apply the attr in typedef
__attribute__((noreturn))
#endif // __clang__
void (cxa_throw_t)(void *ex, CXA_THROW_INFO_T *info, void (*dest)(void*));
#endif // !STATICLIB

extern "C"
__attribute__((noreturn))
void CXA_THROW(void *ex, CXA_THROW_INFO_T *info, void (*dest)(void*))
{

  int status;
  char *dsym = abi::__cxa_demangle(((const std::type_info*)info)->name(), NULL, NULL, &status);
  tools::log_stack_trace((std::string("Exception: ")+((!status && dsym) ? dsym : (const char*)info)).c_str());
  free(dsym);

#ifndef STATICLIB
#ifndef __clang__ // for GCC the attr can't be applied in typedef like for clang
  __attribute__((noreturn))
#endif // !__clang__
   cxa_throw_t *__real___cxa_throw = (cxa_throw_t*)dlsym(RTLD_NEXT, "__cxa_throw");
#endif // !STATICLIB
  __real___cxa_throw(ex, info, dest);
}

namespace
{
  std::string stack_trace_log;
}

namespace tools
{

void set_stack_trace_log(const std::string &log)
{
  stack_trace_log = log;
}

void log_stack_trace(const char *msg)
{
#ifdef USE_UNWIND
  unw_context_t ctx;
  unw_cursor_t cur;
  unw_word_t ip, off;
  unsigned level;
  char sym[512], *dsym;
  int status;
  const char *log = stack_trace_log.empty() ? NULL : stack_trace_log.c_str();
#endif

  if (msg)
    ST_LOG(msg);
  ST_LOG("Unwound call stack:");

#ifdef USE_UNWIND
  if (unw_getcontext(&ctx) < 0) {
    ST_LOG("Failed to create unwind context");
    return;
  }
  if (unw_init_local(&cur, &ctx) < 0) {
    ST_LOG("Failed to find the first unwind frame");
    return;
  }
  for (level = 1; level < 999; ++level) { // 999 for safety
    int ret = unw_step(&cur);
    if (ret < 0) {
      ST_LOG("Failed to find the next frame");
      return;
    }
    if (ret == 0)
      break;
    if (unw_get_reg(&cur, UNW_REG_IP, &ip) < 0) {
      ST_LOG("  " << std::setw(4) << level);
      continue;
    }
    if (unw_get_proc_name(&cur, sym, sizeof(sym), &off) < 0) {
      ST_LOG("  " << std::setw(4) << level << std::setbase(16) << std::setw(20) << "0x" << ip);
      continue;
    }
    dsym = abi::__cxa_demangle(sym, NULL, NULL, &status);
    ST_LOG("  " << std::setw(4) << level << std::setbase(16) << std::setw(20) << "0x" << ip << " " << (!status && dsym ? dsym : sym) << " + " << "0x" << off);
    free(dsym);
  }
#else
  std::stringstream ss;
  ss << el::base::debug::StackTrace();
  std::vector<std::string> lines;
  std::string s = ss.str();
  boost::split(lines, s, boost::is_any_of("\n"));
  for (const auto &line: lines)
    ST_LOG(line);
#endif
}

}  // namespace tools
