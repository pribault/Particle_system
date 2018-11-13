#pragma once

#include "particle_system.h"
#include "pribault.h"
#include "call.h"

template    <typename T>
class   Buffer
{
    public:
        Buffer(const size_t &count) : _size(sizeof(T) * count)
        {
            cl_int  clError;
            GLenum  glError;

            /*
            **  OpenGL
            */

           _log << "creating Buffer (" << strSize(_size) << ")" << std::endl;

            glGenBuffers(1, &_glId);
            glError = glGetError();
            if (glError != GL_NO_ERROR)
                throw (pribault::BasicException(std::string("OpenGL error on glGenBuffers: ").append(std::to_string(glError))));

            glBindBuffer(GL_ARRAY_BUFFER, _glId);
            glError = glGetError();
            if (glError != GL_NO_ERROR)
            {
                glDeleteBuffers(1, &_glId);
                throw (pribault::BasicException(std::string("OpenGL error on glBindBuffer: ").append(std::to_string(glError))));
            }

            glBufferData(GL_ARRAY_BUFFER, _size, NULL, GL_STREAM_DRAW);
            glError = glGetError();
            if (glError != GL_NO_ERROR)
            {
                glDeleteBuffers(1, &_glId);
                throw (pribault::BasicException(std::string("OpenGL error on glBufferData: ").append(std::to_string(glError))));
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glError = glGetError();
            if (glError != GL_NO_ERROR)
            {
                glDeleteBuffers(1, &_glId);
                throw (pribault::BasicException(std::string("OpenGL error on glBindBuffer: ").append(std::to_string(glError))));
            }

            /*
            **  OpenCL
            */

            _clId = clCreateFromGLBuffer(window->getContext(), CL_MEM_READ_WRITE, _glId, &clError);
            if (clError != CL_SUCCESS)
            {
                glDeleteBuffers(1, &_glId);
                throw (pribault::BasicException(std::string("OpenCL error on clCreateFromGLBuffer: ").append(std::to_string(clError))));
            }

            _log << "buffer created" << std::endl;
        }
        ~Buffer(void)
        {
			CL_CALL(clReleaseMemObject(_clId));
            GL_CALL(glDeleteBuffers(1, &_glId));
            _log << "buffer deleted" << std::endl;
        }

        size_t  getSize(void)
        {
            return (_size);
        }
        void    bind(void)
        {
            GLenum  error;

            glBindBuffer(GL_ARRAY_BUFFER, _glId);
            error = glGetError();
            if (error != GL_NO_ERROR)
                throw (pribault::BasicException(std::string("OpenGL error on glBindBuffer: ").append(std::to_string(error))));
        }

        cl_mem    &getMem(void)
        {
            return (_clId);
        }
        void    acquire(void)
        {
            cl_int  error;

            error = clEnqueueAcquireGLObjects(window->getQueue(), 1, &_clId, 0, NULL, NULL);
            if (error != CL_SUCCESS)
                throw (pribault::BasicException(std::string("OpenCL error on clEnqueueAcquireGLObjects: ").append(std::to_string(error))));
        }
        void    release(void)
        {
            cl_int  error;

            error = clEnqueueReleaseGLObjects(window->getQueue(), 1, &_clId, 0, NULL, NULL);
            if (error != CL_SUCCESS)
                throw (pribault::BasicException(std::string("OpenCL error on clEnqueueAcquireGLObjects: ").append(std::to_string(error))));
        }
    private:
        GLuint  _glId;
        cl_mem  _clId;
        size_t  _size;
};
