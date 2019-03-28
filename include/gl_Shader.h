#pragma once

#include <GL/glew.h>
#include <string>

namespace	gl
{
	class	Shader
	{
		friend	class Program;
		public:
			Shader(const GLenum &type, const std::string &file);
			~Shader(void);
		private:
			const GLuint	&getId(void) const noexcept;
			GLuint	_id;
	};
}
