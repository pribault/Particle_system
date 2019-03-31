#include "Window.h"
#include "Log.h"
#include "pribault.h"

#if defined (__APPLE__)
# include <OpenGL/CGLCurrent.h>
# include <OpenCL/opencl.h>
#elif defined (__linux__)
# include <GL/glx.h>
#elif (defined _WIN32 || defined _WIN64)
# include <windows.h>
#endif

using namespace	pribault;

pribault::Window::Window(void) : Window("default", 640, 480)
{
}

pribault::Window::Window(const std::string &title) : Window(title, 640, 480)
{
}

pribault::Window::Window(const int &width, const int &height) : Window("default", width, height)
{
}

pribault::Window::Window(const std::string &title, const int &width, const int &height) : _title(title), _width(width), _height(height), _pause(false), _in(false)
{
	std::string buffer;
	size_t		returnedSize;
	cl_int		error;
	GLenum		ret;

	/*
	**	SDL2 / OpenGL initialization
	*/

	if (!(SDL_WasInit(0) & SDL_INIT_VIDEO))
	{
		_log << "initializing SDL2" << std::endl;
		if (SDL_Init(SDL_INIT_VIDEO))
			throw (pribault::BasicException(std::string("cannot initialize SDL2: ").append(SDL_GetError())));
	}
	else
		_log << "SDL2 already initialized" << std::endl;
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,	OPENGL_VERSION_MAJOR)			< 0 ||
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,	OPENGL_VERSION_MINOR)			< 0 ||
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,			1)								< 0 ||
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,	SDL_GL_CONTEXT_PROFILE_CORE)	< 0)
		throw (pribault::BasicException(std::string("cannot set OpenGL attribute: ").append(SDL_GetError())));
	if (!(_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)))
		throw (pribault::BasicException(std::string("cannot create window: ").append(SDL_GetError())));
	if (!(_context = SDL_GL_CreateContext(_window)))
	{
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot create context: ").append(SDL_GetError())));
	}
	if (SDL_GL_SetSwapInterval(VSYNC))
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot set opengl swap interval: ").append(SDL_GetError())));
	}

	/*
	**	Glew initialization
	*/

#ifndef __APPLE__
	if ((ret = glewInit()) != GLEW_OK)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot initialize glew: ").append((const char *)glewGetErrorString(ret))));
	}
#endif

	/*
	**	OpenGL initialization
	*/

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &_vao);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("OpenGL error on glGenVertexArrays: ").append(std::to_string(error))));
	}

	glBindVertexArray(_vao);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("OpenGL error on glBindVertexArray: ").append(std::to_string(error))));
	}

	glEnableVertexAttribArray(0);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("OpenGL error on glEnableVertexAttribArray: ").append(std::to_string(error))));
	}

	glEnableVertexAttribArray(1);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("OpenGL error on glEnableVertexAttribArray: ").append(std::to_string(error))));
	}

	/*
	**	OpenCL initialization
	*/

	if ((error = clGetPlatformIDs(1, &_clPlatform, NULL)) != CL_SUCCESS)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot get platform ids: ").append(std::to_string(error))));
	}

	//	platform name

	buffer.resize(128);
	if ((error = clGetPlatformInfo(_clPlatform, CL_PLATFORM_NAME, buffer.length(), &buffer[0], &returnedSize)) != CL_SUCCESS)
	{
		_log << "cannot get platform name" << std::endl;
	}
	else
	{
		buffer.resize(returnedSize);
		_log << "platform name: " << buffer << std::endl;
	}

	//	platform version

	buffer.resize(128);
	if ((error = clGetPlatformInfo(_clPlatform, CL_PLATFORM_VERSION, buffer.length(), &buffer[0], &returnedSize)) != CL_SUCCESS)
	{
		_log << "cannot get platform version" << std::endl;
	}
	else
	{
		buffer.resize(returnedSize);
		_log << "platform version: " << buffer << std::endl;
	}

	if ((error = clGetDeviceIDs(_clPlatform, DEVICE, 1, &_clDevice, NULL)) != CL_SUCCESS)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot get device ids: ").append(std::to_string(error))));
	}

	//	device name

	buffer.resize(128);
	if ((error = clGetDeviceInfo(_clDevice, CL_DEVICE_NAME, buffer.length(), &buffer[0], &returnedSize)) != CL_SUCCESS)
	{
		_log << "cannot get device name" << std::endl;
	}
	else
	{
		buffer.resize(returnedSize);
		_log << "device name: " << buffer << std::endl;
	}

	//	device version

	buffer.resize(128);
	if ((error = clGetDeviceInfo(_clDevice, CL_DEVICE_VERSION, buffer.length(), &buffer[0], &returnedSize)) != CL_SUCCESS)
	{
		_log << "cannot get device version" << std::endl;
	}
	else
	{
		buffer.resize(returnedSize);
		_log << "device version: " << buffer << std::endl;
	}

	//	device extensions

	buffer.resize(4096);
	if ((error = clGetDeviceInfo(_clDevice, CL_DEVICE_EXTENSIONS, buffer.length(), &buffer[0], &returnedSize)) != CL_SUCCESS)
	{
		_log << "cannot get device extensions" << std::endl;
	}
	else
	{
		buffer.resize(returnedSize);
		_log << "device extensions: " << buffer << std::endl;

		if (buffer.find(SHARING_EXTENSION) == std::string::npos)
		{
			SDL_GL_DeleteContext(_context);
			SDL_DestroyWindow(_window);
			throw (pribault::BasicException(std::string("device doesn't support ") + SHARING_EXTENSION));
		}
	}

#if defined __APPLE__

	static cl_context_properties		properties[] = {
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		0,
		0
	};
	static cl_queue_properties_APPLE	queue_properties = 0;

	properties[1] = (cl_context_properties)CGLGetShareGroup(
		CGLGetCurrentContext());
	_clContext = clCreateContext(properties, 1, &_clDevice, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot create opencl context: ").append(std::to_string(error))));
	}
	_clQueue = clCreateCommandQueueWithPropertiesAPPLE(_clContext, _clDevice, &queue_properties, &error);
	if (error != CL_SUCCESS)
	{
		clReleaseCommandQueue(_clQueue);
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot create opencl context: ").append(std::to_string(error))));
	}

#elif defined __linux__

	static cl_context_properties	properties[] = {
		CL_CONTEXT_PLATFORM, 0,
		CL_GL_CONTEXT_KHR, 0,
		CL_GLX_DISPLAY_KHR, 0,
		0
	};

	properties[1] = (cl_context_properties)_clPlatform;
	properties[3] = (cl_context_properties)glXGetCurrentContext();
	properties[5] = (cl_context_properties)glXGetCurrentDisplay();
	_clContext = clCreateContext(properties, 1, &_clDevice, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot create opencl context: ").append(std::to_string(error))));
	}
	_clQueue = clCreateCommandQueueWithProperties(_clContext, _clDevice, NULL, &error);
	if (error != CL_SUCCESS)
	{
		clReleaseCommandQueue(_clQueue);
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot create opencl context: ").append(std::to_string(error))));
	}

#elif (defined _WIN32 || defined _WIN64)

	static cl_context_properties	properties[] = {
		CL_CONTEXT_PLATFORM, 0,
		CL_GL_CONTEXT_KHR, 0,
		CL_WGL_HDC_KHR, 0,
		0
	};

	properties[1] = (cl_context_properties)_clPlatform;
	properties[3] = (cl_context_properties)wglGetCurrentContext();
	properties[5] = (cl_context_properties)wglGetCurrentDC();
	_clContext = clCreateContext(properties, 1, &_clDevice, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot create opencl context: ").append(std::to_string(error))));
	}
	_clQueue = clCreateCommandQueue(_clContext, _clDevice, NULL, &error);
	if (error != CL_SUCCESS)
	{
		clReleaseCommandQueue(_clQueue);
		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		throw (pribault::BasicException(std::string("cannot create opencl context: ").append(std::to_string(error))));
	}

#endif

	_log << "window '" << _title << "' created (" << _width << "x" << _height << ")" << std::endl;
}

pribault::Window::~Window(void)
{
	clReleaseCommandQueue(_clQueue);
	clReleaseContext(_clContext);
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
	_log << "window deleted" << std::endl;
}

void	pribault::Window::setTitle(const std::string &title)
{
	SDL_SetWindowTitle(_window, title.c_str());
}

void	pribault::Window::setSize(const int &width, const int &height)
{
	_width = width;
	_height = height;
	SDL_SetWindowSize(_window, _width, _height);
}

void	pribault::Window::setWidth(const int &width)
{
	_width = width;
	SDL_SetWindowSize(_window, _width, _height);
}

void	pribault::Window::setHeight(const int &height)
{
	_height = height;
	SDL_SetWindowSize(_window, _width, height);
}

void	pribault::Window::updateSize(const int &width, const int &height)
{
	_width = width;
	_height = height;
}

const std::string	&pribault::Window::getTitle(void) const
{
	return (_title);
}

const int			&pribault::Window::getWidth(void) const
{
	return (_width);
}

const int			&pribault::Window::getHeight(void) const
{
	return (_height);
}

void	pribault::Window::pause(void)
{
	_pause = true;
}

void	pribault::Window::play(void)
{
	_pause = false;
}

bool	pribault::Window::isPaused(void) const
{
	return (_pause);
}

void	pribault::Window::setIn(void)
{
	_log << "cursor in" << std::endl;
	_in = true;
}

void	pribault::Window::setOut(void)
{
	_log << "cursor out" << std::endl;
	_in = false;
}

bool	pribault::Window::isIn(void) const
{
	return (_in);
}

bool	pribault::Window::handleEvent(SDL_Event *event) const
{
	return (SDL_PollEvent(event));
}

void	pribault::Window::swapBuffers(void)
{
	SDL_GL_SwapWindow(_window);
}

cl_command_queue		&pribault::Window::getQueue(void)
{
	return (_clQueue);
}

const cl_context		&pribault::Window::getContext(void) const
{
	return (_clContext);
}

const cl_device_id		&pribault::Window::getDevice(void) const
{
	return (_clDevice);
}

void	pribault::Window::glFinish(void)
{
	GLenum	error;

	::glFinish();
	error = glGetError();
	if (error != GL_NO_ERROR)
		throw (pribault::BasicException(std::string("OpenGL error on glUseProgram: ").append(std::to_string(error))));
}

void	pribault::Window::clFinish(void)
{
	cl_int	error;

	error = ::clFinish(_clQueue);
	if (error != CL_SUCCESS)
		throw (pribault::BasicException(std::string("OpenCL error on clFinish: ").append(std::to_string(error))));
}
