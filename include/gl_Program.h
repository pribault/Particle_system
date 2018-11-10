#pragma once

#include "gl_Shader.h"

namespace   gl
{
    class   Program
    {
        public:
            Program(void);
            ~Program(void);

            void    attach(const Shader &shader);
            void    link(void);
            void    use(void);
        private:
            GLuint  _id;
    };
}
