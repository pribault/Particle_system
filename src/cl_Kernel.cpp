#include "cl_Kernel.h"
#include "pribault.h"
#include "call.h"
#include "Log.h"

using namespace	cl;

Kernel::Kernel(const Program &program, const std::string &name)
{
	cl_int	error;

	_log << "creating '" << name << "' kernel" << std::endl;
	_id = clCreateKernel(program._id, name.c_str(), &error);
	if (error != CL_SUCCESS)
		throw (pribault::BasicException(std::string("OpenCL error on clCreateKernel: ").append(std::to_string(error))));
	_log << "kernel created" << std::endl;
}

Kernel::~Kernel(void)
{
	cl_int	error;

	error = clReleaseKernel(_id);
	if (error != CL_SUCCESS)
		_log << "openCL error on clReleaseKernel: " << error << std::endl;
	_log << "OpenCL kernel deleted" << std::endl;
}

void	Kernel::enqueue(const size_t &n)
{
	cl_int	error;

	error = clEnqueueNDRangeKernel(window->getQueue(), _id, 1, nullptr, &n, nullptr, 0, nullptr, nullptr);
	if (error != CL_SUCCESS)
		throw (pribault::BasicException(std::string("OpenCL error on clEnqueueNDRangeKernel: ").append(std::to_string(error))));
}
