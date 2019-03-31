#pragma once

#ifdef __APPLE__
# include <OpenGL/gl3.h>
#else
# include <GL/glew.h>
#endif
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
