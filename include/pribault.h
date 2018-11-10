#pragma once

#include <string>
#include <exception>
#include <iostream>
#include <string.h>

#define ABS(x)			(((x) < 0) ? -(x) : (x))

#define BIT(n)		(1 << (n))
#define ISSET(x, n)	((x) & (n))
#define SET(x, n)		((x) | (n))

#define PAIR_ENUM(e)	std::make_pair(e, #e)

namespace	pribault
{
	class	BasicException : public std::exception
	{
		public:
			BasicException(const std::string &msg) : _msg(msg) {}
			virtual	~BasicException(void) {}

			virtual const char*	what() const noexcept
			{
				return (_msg.c_str());
			}

			BasicException	&operator=(const std::exception &)
			{
				_msg = "default";
				return (*this);
			}
		private:
			std::string	_msg;
	};

	template	<typename T>
	int			memcmp(const T &a, const T &b)
	{
		return (::memcmp((const void *)&a, (const void *)&b, sizeof(T)));
	}

	template	<typename T>
	void		memdump(const T &src)
	{
		size_t i;

		for (i = 0; i < sizeof(T); i++)
			if (!((i + 1) % 4))
				std::cout << std::to_string(((const uint8_t *)&src)[i]) << std::endl;
			else
				std::cout << std::to_string(((const uint8_t *)&src)[i]) << " ";
		if (i % 4)
			std::cout << std::endl;
	}
}
