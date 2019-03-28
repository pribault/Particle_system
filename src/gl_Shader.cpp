#include "gl_Shader.h"
#include "pribault.h"
#include "call.h"

#include <fstream>

using namespace	gl;

Shader::Shader(const GLenum &type, const std::string &file)
{
	char			buffer[8192];
	std::string		str;
	std::ifstream	in;
	const char		*tmp;

	GLenum	error;

	_log << "creating OpenGL shader from '" << file << "'" << std::endl;
	_id = glCreateShader(type);
	error = glGetError();
	if (error != GL_NO_ERROR)
		throw (pribault::BasicException(std::string("OpenGL error on glCreateShader: ").append(std::to_string(error))));

	in.open(file, std::ifstream::binary);
	if (in.is_open())
	{
		while (!in.eof())
		{
			in.read((char *)&buffer, sizeof(buffer));
			str.append((char *)&buffer, in.gcount());
		}
		in.close();

		_log << "creating shader from source" << std::endl;

		tmp = str.c_str();
		glShaderSource(_id, 1, &tmp, NULL);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			GL_CALL(glDeleteShader(_id));
			throw (pribault::BasicException(std::string("OpenGL error on glShaderSource: ").append(std::to_string(error))));
		}

		_log << "compiling shader" << std::endl;

		glCompileShader(_id);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			GL_CALL(glDeleteShader(_id));
			throw (pribault::BasicException(std::string("OpenGL error on glCompileShader: ").append(std::to_string(error))));
		}
	}
	else
	{
		GL_CALL(glDeleteShader(_id));
		throw (pribault::BasicException(std::string("cannot open '").append(file).append("'")));
	}

	_log << "shader created" << std::endl;
}

Shader::~Shader(void)
{
	GL_CALL(glDeleteShader(_id));
	_log << "OpenGL shader deleted" << std::endl;
}

const GLuint	&Shader::getId(void) const noexcept
{
	return (_id);
}

