#pragma once

#include "cl_Program.h"
#include "Buffer.h"

#include <map>

namespace	cl
{
	class	Kernel
	{
		public:
			Kernel(const Program &program, const std::string &name);
			~Kernel(void);

			template	<typename T>
			void	setArg(Buffer<T> &buffer, cl_uint index)
			{
				cl_int	error;

				error = clSetKernelArg(_id, index, sizeof(cl_mem), &buffer.getMem());
				if (error != GL_NO_ERROR)
					throw (pribault::BasicException(std::string("OpenCL error on clSetKernelArg: ").append(std::to_string(error))));
			}
			template	<typename T>
			void	setArg(const T &arg, cl_uint index)
			{
				cl_int	error;

				error = clSetKernelArg(_id, index, sizeof(T), &arg);
				if (error != GL_NO_ERROR)
					throw (pribault::BasicException(std::string("OpenCL error on clSetKernelArg: ").append(std::to_string(error))));
			}

			void	enqueue(const size_t &n);
		private:
			std::map<cl_uint, cl_mem *> _map;
			cl_kernel					_id;
	};
}
