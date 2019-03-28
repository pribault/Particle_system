#pragma once

#define CL_TARGET_OPENCL_VERSION	200

#if defined __APPLE__
# include <OpenCL/cl.h>
#else
# include <CL/opencl.h>
#endif

#include <string>

namespace	cl
{
	class	Program
	{
		friend class	Kernel;
		public:
			Program(const std::string &file);
			~Program(void);
		private:
			cl_program	_id;
	};
}
