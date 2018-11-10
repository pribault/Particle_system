#include "Log.h"

Log	_log("log");

Log::Log(const std::string &file)
{
	try
	{
		_file.open(file.c_str(), std::ofstream::out | std::ofstream::app);
		_file << "___ Log start ___" << std::endl;
#ifdef LOG_TERM
		std::cout << "___ Log start ___" << std::endl;
#endif
	}
	catch (const std::exception &)
	{
		throw ;
	}
	_start = true;
}

Log::~Log()
{
	_file << "___ Log end ___" << std::endl;
#ifdef LOG_TERM
	std::cout << "___ Log end ___" << std::endl;
#endif
	_file.close();
}

Log		&Log::operator<<(std::ostream& (*pf)(std::ostream&))
{
	if (!pf)
		throw (std::exception());
	try
	{
#ifdef LOG_TERM
		pf(std::cout);
#endif
		pf(_file);
	}
	catch (const std::exception &)
	{
		throw ;
	}
	_start = true;
	return (*this);
}
