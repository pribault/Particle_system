#include "gl_Program.h"
#include "pribault.h"
#include "call.h"

#include <glm/gtc/type_ptr.hpp>

using namespace	gl;

Program::Program(void)
{
	GLenum	error;

	_log << "creating OpenGL program" << std::endl;
	_id = glCreateProgram();
	error = glGetError();
	if (error != GL_NO_ERROR)
		throw (pribault::BasicException(std::string("OpenGL error on glCreateProgram: ").append(std::to_string(error))));
	_log << "program created" << std::endl;
}

Program::~Program(void)
{
	GL_CALL(glDeleteProgram(_id));
	_log << "OpenGL program deleted" << std::endl;
}

void	Program::attach(const Shader &shader)
{
	GLenum	error;

	_log << "attaching shader to program" << std::endl;
	glAttachShader(_id, shader._id);
	error = glGetError();
	if (error != GL_NO_ERROR)
		throw (pribault::BasicException(std::string("OpenGL error on glAttachShader: ").append(std::to_string(error))));
}

void	Program::link(void)
{
	GLenum	error;

	_log << "linking program" << std::endl;
	glLinkProgram(_id);
	error = glGetError();
	if (error != GL_NO_ERROR)
		throw (pribault::BasicException(std::string("OpenGL error on glLinkProgram: ").append(std::to_string(error))));
}

void	Program::use(void)
{
	GLenum	error;

	//	_log << "using program" << std::endl;
	glUseProgram(_id);
	error = glGetError();
	if (error != GL_NO_ERROR)
		throw (pribault::BasicException(std::string("OpenGL error on glUseProgram: ").append(std::to_string(error))));
}

void	Program::bind(const std::string &name, const glm::mat4 &mat)
{
	GL_CALL(glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)));
}
