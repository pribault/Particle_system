#include "particle_system.h"
#include "cl_Program.h"
#include "pribault.h"
#include "call.h"
#include "Log.h"

#include <fstream>

using namespace cl;

Program::Program(const std::string &file)
{
    static const std::map<cl_build_status, std::string> strStatus({
        PAIR_ENUM(CL_BUILD_NONE),
        PAIR_ENUM(CL_BUILD_ERROR),
        PAIR_ENUM(CL_BUILD_SUCCESS),
        PAIR_ENUM(CL_BUILD_IN_PROGRESS)
    });
    cl_build_status status;
    cl_int          error, error2;
    char            buffer[8192];
    std::string     str;
    std::ifstream   in;
    const char      *tmp;
    size_t          returnedSize;

    in.open(file, std::ifstream::binary);
    if (in.is_open())
    {
        while (!in.eof())
        {
            in.read((char *)&buffer, sizeof(buffer));
            str.append((char *)&buffer, in.gcount());
        }
        in.close();

        _log << "creating OpenCL program from source" << std::endl;

        tmp = str.c_str();
        _id = clCreateProgramWithSource(window->getContext(), 1, &tmp, NULL, &error);
        if (error != CL_SUCCESS)
            throw (pribault::BasicException(std::string("OpenCL error on clCreateProgramWithSource: ").append(std::to_string(error))));

        error = clBuildProgram(_id, 1, &window->getDevice(), "", NULL, NULL);

        error2 = clGetProgramBuildInfo(_id, window->getDevice(), CL_PROGRAM_BUILD_STATUS, sizeof(status), &status, &returnedSize);
        if (error2 != CL_SUCCESS)
        {
            CL_CALL(clReleaseProgram(_id));
            throw (pribault::BasicException(std::string("OpenCL error on clGetProgramBuildInfo: ").append(std::to_string(error2))));
        }
        try
        {
            _log << "build status: " << strStatus.at(status) << std::endl;
        }
        catch (const std::exception &e)
        {
            _log << "invalid device status" << std::endl;
        }

        error2 = clGetProgramBuildInfo(_id, window->getDevice(), CL_PROGRAM_BUILD_LOG, sizeof(buffer), &buffer, &returnedSize);
        if (error2 != CL_SUCCESS)
        {
            CL_CALL(clReleaseProgram(_id));
            throw (pribault::BasicException(std::string("OpenCL error on clGetProgramBuildInfo: ").append(std::to_string(error2))));
        }
        _log << std::string((char *)&buffer, returnedSize) << std::endl;

        if (error != CL_SUCCESS)
        {
            CL_CALL(clReleaseProgram(_id));
            throw (pribault::BasicException(std::string("OpenCL error on clBuildProgram: ").append(std::to_string(error))));
        }
    }
    else
    {
        throw (pribault::BasicException(std::string("cannot open '").append(file).append("'")));
    }
}

Program::~Program(void)
{
    CL_CALL(clReleaseProgram(_id));
    _log << "OpenCL program deleted" << std::endl;
}
