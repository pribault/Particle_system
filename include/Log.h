#pragma once

#include <string>
#include <fstream>
#include <time.h>

#define LOG_TERM

#ifdef LOG_TERM
# include <iostream>
#endif

#ifndef __linux__
# pragma warning(disable : 4996)
#endif

class Log
{
	public:
		Log(const std::string &file);
		~Log();

		Log		&operator<<(std::ostream& (*pf)(std::ostream&));

		template	<typename T>
		Log		&operator<<(const T &s)
		{
			if (_start == true)
			{
				std::string	prefix;
				time_t		t = time(0);

				prefix.resize(20);
				prefix.resize(strftime(const_cast<char *>(prefix.c_str()), prefix.length(), "%G%m%d_%H%M%S", localtime(&t)));
				try
				{
#ifdef LOG_TERM
					std::cout << "[" << prefix << "] " << s;
#endif
					_file << "[" << prefix << "] " << s;
				}
				catch (const std::exception &)
				{
					throw ;
				}
				_start = false;
			}
			else
			{
				try
				{
#ifdef LOG_TERM
					std::cout << s;
#endif
					_file << s;
				}
				catch (const std::exception &)
				{
					throw ;
				}
			}
			return (*this);
		}
	private:
		std::ofstream	_file;
		bool			_start;
};

extern Log	_log;
