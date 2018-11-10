#include "particle_system.h"

void    drawParticles(void)
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    particlePositions->bind();
    GL_CALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL));
    particleColors->bind();
    GL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL));
    GL_CALL(glDrawArrays(GL_POINTS, 0, particles));
    window->glFinish();
    window->swapBuffers();
}
