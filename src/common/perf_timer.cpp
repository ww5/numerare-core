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

#include <vector>
#include "misc_os_dependent.h"
#include "perf_timer.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "perf"

namespace
{
  uint64_t get_tick_count()
  {
#if defined(__x86_64__)
    uint32_t hi, lo;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return (((uint64_t)hi) << 32) | (uint64_t)lo;
#else
    return epee::misc_utils::get_ns_count();
#endif
  }

#ifdef __x86_64__
  uint64_t get_ticks_per_ns()
  {
    uint64_t t0 = epee::misc_utils::get_ns_count(), t1;
    uint64_t r0 = get_tick_count();

    while (1)
    {
      t1 = epee::misc_utils::get_ns_count();
      if (t1 - t0 > 1*1000000000) break; // work one second
    }

    uint64_t r1 = get_tick_count();
    uint64_t tpns256 = 256 * (r1 - r0) / (t1 - t0);
    return tpns256 ? tpns256 : 1;
  }
#endif

#ifdef __x86_64__
  uint64_t ticks_per_ns = get_ticks_per_ns();
#endif

  uint64_t ticks_to_ns(uint64_t ticks)
  {
#if defined(__x86_64__)
    return 256 * ticks / ticks_per_ns;
#else
    return ticks;
#endif
  }
}

namespace tools
{

el::Level performance_timer_log_level = el::Level::Debug;

static __thread std::vector<PerformanceTimer*> *performance_timers = NULL;

void set_performance_timer_log_level(el::Level level)
{
  if (level != el::Level::Debug && level != el::Level::Trace && level != el::Level::Info
   && level != el::Level::Warning && level != el::Level::Error && level != el::Level::Fatal)
  {
    MERROR("Wrong log level: " << el::LevelHelper::convertToString(level) << ", using Debug");
    level = el::Level::Debug;
  }
  performance_timer_log_level = level;
}

PerformanceTimer::PerformanceTimer(const std::string &s, uint64_t unit, el::Level l): name(s), unit(unit), level(l), started(false), paused(false)
{
  ticks = get_tick_count();
  if (!performance_timers)
  {
    MLOG(level, "PERF             ----------");
    performance_timers = new std::vector<PerformanceTimer*>();
  }
  else
  {
    PerformanceTimer *pt = performance_timers->back();
    if (!pt->started && !pt->paused)
    {
      size_t size = 0; for (const auto *tmp: *performance_timers) if (!tmp->paused) ++size;
      MLOG(pt->level, "PERF           " << std::string((size-1) * 2, ' ') << "  " << pt->name);
      pt->started = true;
    }
  }
  performance_timers->push_back(this);
}

PerformanceTimer::~PerformanceTimer()
{
  performance_timers->pop_back();
  if (!paused)
    ticks = get_tick_count() - ticks;
  char s[12];
  snprintf(s, sizeof(s), "%8llu  ", (unsigned long long)(ticks_to_ns(ticks) / (1000000000 / unit)));
  size_t size = 0; for (const auto *tmp: *performance_timers) if (!tmp->paused || tmp==this) ++size;
  MLOG(level, "PERF " << s << std::string(size * 2, ' ') << "  " << name);
  if (performance_timers->empty())
  {
    delete performance_timers;
    performance_timers = NULL;
  }
}

void PerformanceTimer::pause()
{
  if (paused)
    return;
  ticks = get_tick_count() - ticks;
  paused = true;
}

void PerformanceTimer::resume()
{
  if (!paused)
    return;
  ticks = get_tick_count() - ticks;
  paused = false;
}

}
