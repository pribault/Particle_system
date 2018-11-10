#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

#if defined __APPLE__
# include <OpenCL/cl.h>
#elif defined __linux__
# include <CL/opencl.h>
#endif

#define OPENGL_VERSION_MAJOR    4
#define OPENGL_VERSION_MINOR    0

#define VSYNC                   1

#define DEVICE                  CL_DEVICE_TYPE_GPU

namespace   pribault
{
    class   Window
    {
        public:
            Window(void);
            Window(const std::string &title);
            Window(const int &width, const int &height);
            Window(const std::string &title, const int &width, const int &height);
            ~Window(void);

            void    setTitle(const std::string &title);
            void    setSize(const int &width, const int &height);
            void    setWidth(const int &width);
            void    setHeight(const int &height);
            void    updateSize(const int &width, const int &height);

            const std::string   &getTitle(void) const;
            const int           &getWidth(void) const;
            const int           &getHeight(void) const;

            void    pause(void);
            void    play(void);
            bool    isPaused(void) const;

            void    setIn(void);
            void    setOut(void);
            bool    isIn(void) const;

            bool    handleEvent(SDL_Event *event) const;

            void    swapBuffers(void);
            void    glFinish(void);

            cl_command_queue        &getQueue(void);
            const cl_context        &getContext(void) const;
            const cl_device_id      &getDevice(void) const;
            void                    clFinish(void);
        private:
            SDL_Window          *_window;
            SDL_GLContext       _context;
            std::string         _title;
            int                 _width;
            int                 _height;
            bool                _pause;
            bool                _in;
            GLuint              _vao;
        	cl_platform_id		_clPlatform;
    	    cl_device_id		_clDevice;
    	    cl_context			_clContext;
        	cl_command_queue	_clQueue;
    };
}
