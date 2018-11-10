#pragma once

#include "Window.h"
#include "gl_Program.h"

#include <map>
#include <glm/vec2.hpp>

#define TITLE   "Particle_System"
#define FPS_LOCK    60

void    drawParticles(void);

std::string strSize(size_t size);

/*
**  events
*/

typedef void    (*t_event_handler)(const SDL_Event &);

void    windowEventHandler(const SDL_Event &event);
void    KeyEventHandler(const SDL_Event &event);
void    MouseMotionEventHandler(const SDL_Event &event);
void    MouseButtonEventHandler(const SDL_Event &event);
void    MouseWheelEventHandler(const SDL_Event &event);

bool    isPressed(const SDL_Keycode &key);

void    handleEvents(void);

/*
**  globals
*/

extern pribault::Window     *window;
extern std::map<int, bool>  keyboard;
extern std::map<int, bool>  mouse;
extern glm::vec2            cursor;

extern gl::Program          *renderer;

extern size_t               particles;

#include "cl_Kernel.h"

template    <typename T>
class       Buffer;

extern Buffer<cl_float3>    *particlePositions;
extern Buffer<cl_float3>    *particleSpeeds;
extern Buffer<cl_float4>    *particleColors;
