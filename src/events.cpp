#include "particle_system.h"
#include "Log.h"
#include "pribault.h"
#include <math.h>

void	windowEventHandler(const SDL_Event &e)
{
	SDL_WindowEvent event = e.window;

	switch (event.event)
	{
		case SDL_WINDOWEVENT_HIDDEN:
		{
			window->pause();
			break ;
		}
		case SDL_WINDOWEVENT_SHOWN:
		{
			window->play();
			break ;
		}
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		{
			window->updateSize(event.data1, event.data2);
			break ;
		}
		case SDL_WINDOWEVENT_RESIZED:
		{
			window->updateSize(event.data1, event.data2);
			break ;
		}
		case SDL_WINDOWEVENT_CLOSE:
		{
			exit(EXIT_SUCCESS);
			break ;
		}
		case SDL_WINDOWEVENT_ENTER:
		{
			window->setIn();
			break ;
		}
		case SDL_WINDOWEVENT_LEAVE:
		{
			window->setOut();
			break ;
		}
		default:
		{
			_log << "unhandled window event, type=" << (size_t)event.event << std::endl;
			break ;
		}
	}
}

void	KeyEventHandler(const SDL_Event &e)
{
	SDL_KeyboardEvent	event = e.key;

	switch (event.type)
	{
		case SDL_KEYUP:
		{
			if (event.state == SDL_RELEASED)
			{
				try
				{
					keyboard[event.keysym.sym] = false;
				}
				catch (const std::exception &e)
				{
					_log << "cannot set key " << event.keysym.sym << " to down" << std::endl;
				}
			}
			break ;
		}
		case SDL_KEYDOWN:
		{
			if (event.state == SDL_PRESSED)
			{
				if (!isPressed(event.keysym.sym))
				{
					try
					{
						keyboard[event.keysym.sym] = true;
					}
					catch (const std::exception &e)
					{
						_log << "cannot set key " << event.keysym.sym << " to down" << std::endl;
					}
					switch (event.keysym.sym)
					{
						case SDLK_1:
						{
							particleDefaultPositions->acquire();

							init_square->setArg(*particleDefaultPositions, 0);
							init_square->setArg((double)particles, 1);

							init_square->enqueue(particles);

							particleDefaultPositions->release();

							window->clFinish();
							break ;
						}
						case SDLK_2:
						{
							particleDefaultPositions->acquire();

							init_circle->setArg(*particleDefaultPositions, 0);
							init_circle->setArg((double)particles, 1);

							init_circle->enqueue(particles);

							particleDefaultPositions->release();

							window->clFinish();
							break ;
						}
						case SDLK_RETURN:
						case SDLK_KP_ENTER:
						{
							gPause = (gPause) ? false : true;
							break ;
						}
						default:
							break ;
					}
				}
			}
			break ;
		}
		default:
		{
			_log << "unknown keyboard type " << event.type << std::endl;
			break ;
		}
	}
}

void	MouseMotionEventHandler(const SDL_Event &e)
{
	SDL_MouseMotionEvent	event = e.motion;

	cursor.x = event.x * (2 / ((double)window->getWidth() - 1)) - 1;
	cursor.y = event.y * (-2 / ((double)window->getHeight() - 1)) + 1;
	if (window->isIn())
	{
		if (ABS(cursor.x) > 1 ||
			ABS(cursor.y) > 1)
			window->setOut();
	}
	else
	{
		if (ABS(cursor.x) <= 1 &&
			ABS(cursor.y) <= 1)
			window->setIn();
	}
}

void	MouseButtonEventHandler(const SDL_Event &e)
{
	SDL_MouseButtonEvent	event = e.button;

	switch (event.type)
	{
		case SDL_MOUSEBUTTONUP:
		{
			if (event.state == SDL_RELEASED)
			{
				try
				{
					mouse[event.button] = false;
				}
				catch (const std::exception &e)
				{
					_log << "cannot set button " << (size_t)event.button << " to down" << std::endl;
				}
				break ;
			}
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.state == SDL_PRESSED)
			{
				try
				{
					mouse[event.button] = true;
				}
				catch (const std::exception &e)
				{
					_log << "cannot set button " << event.button << " to down" << std::endl;
				}
			}
			break ;
		}
		default:
		{
			_log << "unknown mouse type " << event.type << std::endl;
			break ;
		}
	}
}

void	MouseWheelEventHandler(const SDL_Event &e)
{
	SDL_MouseWheelEvent event = e.wheel;

	_log << "event.y=" << event.y << std::endl;
	if (event.y)
		event.y /= fabs(event.y);
	if (isPressed(SDLK_LCTRL))
	{
		deleteBuffers();
		particles *= pow(2, event.y);
		if (!particles)
		{
			particles = 1;
			_log << "particles number cannot be less than 1, you will not make me segfault today <3" << std::endl;
		}
		initBuffers();
	}
	else if (isPressed(SDLK_c))
	{
		alpha *= pow(2, event.y);

		particleColors->acquire();

		colors_kernels->setArg(*particleColors, 0);
		colors_kernels->setArg((double)particles, 1);
		colors_kernels->setArg(alpha, 2);

		colors_kernels->enqueue(particles);

		particleColors->release();

		window->clFinish();
	}
	else
		gravity *= pow(2, event.y);
}

bool	isPressed(const SDL_Keycode &key)
{
	try
	{
		return (keyboard.at(key));
	}
	catch (const std::exception &e)
	{
		try
		{
			return (mouse.at(key));
		}
		catch (const std::exception &e)
		{
			return (false);
		}
	}
}

void	handleEvents(void)
{
	if (isPressed(SDLK_ESCAPE))
		exit(EXIT_SUCCESS);
}
