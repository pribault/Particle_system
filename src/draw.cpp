#include "particle_system.h"
#include <glm/gtc/matrix_transform.hpp>

void	drawParticles(gl::Program *program, const Camera &camera)
{
	glm::mat4	model((float)1);

	try
	{
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		program->bind("model", model);
		program->bind("view", camera.getView());
		program->bind("projection", camera.getProjection());
		particlePositions->bind();
		GL_CALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL));
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
