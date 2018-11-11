#include "particle_system.h"

void    drawParticles(void)
{
    try
    {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        particlePositions->bind();
        GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL));
        particleColors->bind();
        GL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL));
        GL_CALL(glDrawArrays(GL_POINTS, 0, particles));
        window->glFinish();
        window->swapBuffers();
    }
    catch (const std::exception &e)
    {
        _log << "cannot render particles: " << e.what() << std::endl;
    }
}
