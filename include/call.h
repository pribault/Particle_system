#pragma once

#define DEBUG

#ifdef DEBUG

# include "Log.h"
# include "pribault.h"

# define GL_CALL(call)	\
{\
	call;\
	GLenum glCallError = glGetError();\
	if (glCallError != GL_NO_ERROR)\
        	_log << "OpenGL error (at " << #call << " - " << __FILE__ << " - " << __LINE__ << "): " << glCallError << std::endl;\
}
# define CL_CALL(call)	\
{\
	cl_int clCallError = call;\
	if (clCallError != CL_SUCCESS)\
        	_log << "OpenCL error (at " << #call << " - " << __FILE__ << " - " << __LINE__ << "): " << clCallError << std::endl;\
}
#else
# define GL_CALL(call)		call
# define CL_CALL(call)		call
#endif
