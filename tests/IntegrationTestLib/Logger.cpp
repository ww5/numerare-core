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

#include "Logger.h"

#include <algorithm>
#include <boost/date_time.hpp>

CLogger& CLogger::Instance()
{
	static CLogger theSingleInstance;
	return theSingleInstance;
}
void CLogger::init(LOG_LEVEL log_lvl)
{
  level_names[TRACE]   = "[ TRACE ]";
  level_names[DEBUG]   = "[ DEBUG ]";
  level_names[_ERROR]  = "[ ERROR ]";
  level_names[WARNING] = "[WARNING]";
  level_names[VERBOSE] = "[VERBOSE]";
  log_level = log_lvl;
  indent = 0;
}
void CLogger::Log(const std::string & log_info, LOG_LEVEL log_lvl, int indent_inc)
{
	if(log_lvl>=log_level)
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (indent_inc<0)indent+=indent_inc;
		std::string sindent(std::max(0,indent),' ');
		if (indent_inc>0)indent+=indent_inc;
    (log_lvl<WARNING?std::cout:std::cerr)<<boost::posix_time::to_iso_extended_string(boost::posix_time::second_clock::local_time())<<level_names[log_lvl].c_str()<<sindent<<log_info.c_str()<<std::endl;
	}
}
