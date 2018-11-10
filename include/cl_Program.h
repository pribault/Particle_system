#pragma once

#if defined __APPLE__
# include <OpenCL/cl.h>
#elif defined __linux__
# include <CL/opencl.h>
#endif

#include <string>

namespace   cl
{
    class   Program
    {
        friend class    Kernel;
        public:
            Program(const std::string &file);
            ~Program(void);
        private:
            cl_program  _id;
    };
}
