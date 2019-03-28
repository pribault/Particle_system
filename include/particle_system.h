#pragma once

#include "Window.h"
#include "gl_Program.h"
#include "Camera.h"

#include <map>
#include <glm/vec2.hpp>
#include <glm/trigonometric.hpp>

#define TITLE		"Particle_System"
#define WIDTH		1440
#define HEIGHT		810
#define FPS_LOCK	60
#define FOV			glm::radians((float)45)
#define STEP		6
#define ANGLE		glm::radians((float)50)

void	drawParticles(gl::Program *program, const Camera &camera);

std::string	strSize(size_t size);
void		initBuffers(void);
void		deleteBuffers(void);

/*
**	events
*/

typedef void	(*t_event_handler)(const SDL_Event &);

void	windowEventHandler(const SDL_Event &event);
void	KeyEventHandler(const SDL_Event &event);
void	MouseMotionEventHandler(const SDL_Event &event);
void	MouseButtonEventHandler(const SDL_Event &event);
void	MouseWheelEventHandler(const SDL_Event &event);

bool	isPressed(const SDL_Keycode &key);

void	handleEvents(void);

/*
**	globals
*/

extern pribault::Window		*window;
extern std::map<int, bool>	keyboard;
extern std::map<int, bool>	mouse;
extern glm::vec2			cursor;

extern gl::Program			*renderer;

extern size_t				particles;
extern double				gravity;
extern double				reformer;

#include "cl_Kernel.h"

namespace   cl
{
	class   Kernel;
}

extern cl::Program			*init;
extern cl::Kernel			*init_square;
extern cl::Kernel			*init_circle;
extern cl::Kernel			*init_speed;

extern cl::Program			*move;
extern cl::Kernel			*move_particles;
extern cl::Kernel			*move_particles_to_cursor;

extern cl::Program			*init_colors;
extern cl::Kernel			*init_colors_rainbow;

template	<typename T>
class		Buffer;

extern Buffer<cl_float4>	*particleDefaultPositions;
extern Buffer<cl_float4>	*particlePositions;
extern Buffer<cl_float4>	*particleSpeeds;
extern Buffer<cl_float4>	*particleColors;

extern Buffer<cl_float4>	*temporaryParticleDefaultPositions;
extern Buffer<cl_float4>	*temporaryParticlePositions;
extern Buffer<cl_float4>	*temporaryParticleSpeeds;
extern Buffer<cl_float4>	*temporaryParticleColors;

extern size_t				temporaryParticles;
extern size_t				temporaryParticlesIndex;
