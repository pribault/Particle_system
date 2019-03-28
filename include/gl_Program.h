#pragma once

#include "gl_Shader.h"
#include "glm/mat4x4.hpp"

namespace	gl
{
	class	Program
	{
		public:
			Program(void);
			~Program(void);

			void	attach(const Shader &shader);
			void	link(void);
			void	use(void);

			void	bind(const std::string &name, const glm::mat4 &mat);
		private:
			GLuint	_id;
	};
}
