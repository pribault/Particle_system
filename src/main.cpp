#include "particle_system.h"
#include "Log.h"
#include <cstdlib>
#include <chrono>
#include <signal.h>
//#include <unistd.h>
#include <glm/vec4.hpp>

using namespace pribault;

Window              *window = nullptr;
std::map<int, bool> keyboard;
std::map<int, bool> mouse;
glm::vec2           cursor;

gl::Program         *renderer = nullptr;

cl::Program         *init = nullptr;
cl::Kernel          *init_square = nullptr;
cl::Kernel          *init_circle = nullptr;
cl::Kernel          *init_speed = nullptr;

cl::Program         *move = nullptr;
cl::Kernel          *move_particles = nullptr;
cl::Kernel          *move_particles_to_cursor = nullptr;

cl::Program         *init_colors = nullptr;
cl::Kernel          *init_colors_rainbow = nullptr;
cl::Kernel          *init_colors_burning_ship = nullptr;

Buffer<cl_float2>   *particleDefaultPositions = nullptr;
Buffer<cl_float2>   *particlePositions = nullptr;
Buffer<cl_float2>   *particleSpeeds = nullptr;
Buffer<cl_float4>   *particleColors = nullptr;

size_t              particles = 1000000;

double              gravity = 0.0001;
double              reformer = 0.0002;

std::string strSize(size_t size)
{
    std::string str;

    if (size >= BIT(30))
    {
        if (str.length())
            str.append(" ");
        str.append(std::to_string((size >> 30) & (BIT(10) - 1))).append("Gb");
    }
    if (size >= BIT(20))
    {
        if (str.length())
            str.append(" ");
        str.append(std::to_string((size >> 20) & (BIT(10) - 1))).append("Mb");
    }
    if (size >= BIT(10))
    {
        if (str.length())
            str.append(" ");
        str.append(std::to_string((size >> 10) & (BIT(10) - 1))).append("Kb");
    }
    if (str.length())
        str.append(" ");
    str.append(std::to_string(size & (BIT(10) - 1))).append(" Bytes");
    return (str);
}

void    sigHandler(int sig)
{
    _log << "signal " << sig << " handled" << std::endl;
    exit(EXIT_SUCCESS);
}

void    deleteKernels(void)
{
	if (init_colors_burning_ship)
		delete init_colors_burning_ship;
    if (init_colors_rainbow)
        delete init_colors_rainbow;
    if (init_colors)
        delete init_colors;
    if (move_particles_to_cursor)
        delete move_particles_to_cursor;
    if (move_particles)
        delete move_particles;
    if (move)
        delete move;
    if (init_speed)
        delete init_speed;
    if (init_square)
        delete init_square;
    if (init)
        delete init;
	_log << "kernels deleted" << std::endl;
}

void    deleteBuffers(void)
{
    if (particleDefaultPositions)
        delete particleDefaultPositions;
    if (particlePositions)
        delete particlePositions;
    if (particleSpeeds)
        delete particleSpeeds;
    if (particleColors)
        delete particleColors;
	_log << "buffers deleted" << std::endl;
}

void    atExit(void)
{
    _log << "exiting..." << std::endl;
    deleteBuffers();
    deleteKernels();
    if (renderer)
        delete renderer;
    if (window)
        delete window;
}

void    initKernels(void)
{
    try
    {
        init = new cl::Program("kernels/init_particles.cl");
        move = new cl::Program("kernels/move_particles.cl");
        init_colors = new cl::Program("kernels/init_colors.cl");
    }
    catch (const std::exception &e)
    {
        _log << "cannot create OpenCL Program: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    try
    {
        init_square = new cl::Kernel(*init, "init_square");
        init_circle = new cl::Kernel(*init, "init_circle");
        init_speed = new cl::Kernel(*init, "init_speed");
        move_particles = new cl::Kernel(*move, "move_particles");
        move_particles_to_cursor = new cl::Kernel(*move, "move_particles_to_cursor");
        init_colors_rainbow = new cl::Kernel(*init_colors, "init_colors_rainbow");
		init_colors_burning_ship = new cl::Kernel(*init_colors, "init_colors_burning_ship");
    }
    catch (const std::exception &e)
    {
        _log << "cannot create OpenCL Kernel: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void    initBuffers(void)
{
    try
    {
        particleDefaultPositions = new Buffer<cl_float2>(particles);
        particlePositions = new Buffer<cl_float2>(particles);
        particleSpeeds = new Buffer<cl_float2>(particles);
        particleColors = new Buffer<cl_float4>(particles);

        //  default pos

        particleDefaultPositions->acquire();

        init_square->setArg(*particleDefaultPositions, 0);
        init_square->setArg((double)particles, 1);

        init_square->enqueue(particles);

        //  pos

        particlePositions->acquire();

        clEnqueueCopyBuffer(window->getQueue(), particleDefaultPositions->getMem(), particlePositions->getMem(), 0, 0, particlePositions->getSize(), 0, NULL, NULL);

        particlePositions->release();

        particleDefaultPositions->release();

        //  speed

        particleSpeeds->acquire();

        init_speed->setArg(*particleSpeeds, 0);

        init_speed->enqueue(particles);

        particleSpeeds->release();

        //  colors

        particleColors->acquire();

		init_colors_rainbow->setArg(*particleColors, 0);
		init_colors_rainbow->setArg((double)particles, 1);

		init_colors_rainbow->enqueue(particles);

        particleColors->release();

        window->clFinish();

		_log << "buffers initialized with " << particles << "particles" << std::endl;
    }
    catch (const std::exception &e)
    {
        _log << "cannot create Buffer: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void	printHelp(void)
{
	_log << "commands:" << std::endl
		<< "	- escape: quit program" << std::endl
		<< "	- left click: attract particles to your cursor" << std::endl
		<< "	- mouse wheel: increase or decrease attraction force" << std::endl
		<< "	- lshift + mouse wheel: increase or decrease particle number" << std::endl
		<< "	- 1: set particles default position to square" << std::endl
		<< "	- 2: set particles default position to circle" << std::endl;
}

int     main(int, char **)
{
    static const std::map<Uint32, t_event_handler>  handlers({
        std::make_pair(SDL_WINDOWEVENT, windowEventHandler),
        std::make_pair(SDL_KEYUP, KeyEventHandler),
        std::make_pair(SDL_KEYDOWN, KeyEventHandler),
        std::make_pair(SDL_MOUSEMOTION, MouseMotionEventHandler),
        std::make_pair(SDL_MOUSEBUTTONUP, MouseButtonEventHandler),
        std::make_pair(SDL_MOUSEBUTTONDOWN, MouseButtonEventHandler),
        std::make_pair(SDL_MOUSEWHEEL, MouseWheelEventHandler)
    });
    SDL_Event   event;
    std::chrono::time_point<std::chrono::high_resolution_clock>  now, prev;

    atexit(&atExit);
    signal(SIGINT, &sigHandler);
    signal(SIGTERM, &sigHandler);

    try
    {
        window = new Window(TITLE, 1440, 810);
    }
    catch (const std::exception &e)
    {
        _log << "cannot create new window: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    try
    {
        renderer = new gl::Program();
        renderer->attach(gl::Shader(GL_VERTEX_SHADER, "shaders/vertex.glsl"));
        renderer->attach(gl::Shader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl"));
        renderer->link();
        renderer->use();
    }
    catch (const std::exception &e)
    {
        _log << "cannot create OpenGL Program: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    initKernels();
    initBuffers();

	printHelp();

    while (1)
    {
        while (window->handleEvent(&event))
            try
            {
                handlers.at(event.type)(event);
            }
            catch (const std::exception &e)
            {
                _log << "cannot handle event (type=" << event.type << "):" << e.what();
            }
        handleEvents();

        if (window->isIn() && isPressed(SDL_BUTTON_LEFT))
        {
            try
            {
                particlePositions->acquire();
                particleSpeeds->acquire();
                particleColors->acquire();

                move_particles_to_cursor->setArg(*particlePositions, 0);
                move_particles_to_cursor->setArg(*particleSpeeds, 1);
                move_particles_to_cursor->setArg(*particleColors, 2);
                move_particles_to_cursor->setArg((double)particles, 3);
                move_particles_to_cursor->setArg((double)cursor.x, 4);
                move_particles_to_cursor->setArg((double)cursor.y, 5);
                move_particles_to_cursor->setArg(gravity, 6);

                move_particles_to_cursor->enqueue(particles);

                particlePositions->release();
                particleSpeeds->release();
                particleColors->release();
                window->clFinish();
            }
            catch (const std::exception &e)
            {
                _log << "cannot launch move_particles_to_cursor kernel: " << e.what() << std::endl;
            }
        }
        else
        {
            try
            {
                particleDefaultPositions->acquire();
                particlePositions->acquire();
                particleSpeeds->acquire();
                particleColors->acquire();

                move_particles->setArg(*particlePositions, 0);
                move_particles->setArg(*particleDefaultPositions, 1);
                move_particles->setArg(*particleSpeeds, 2);
                move_particles->setArg(*particleColors, 3);
                move_particles->setArg((double)particles, 4);
                move_particles->setArg((double)reformer, 5);

                move_particles->enqueue(particles);

                particleDefaultPositions->release();
                particlePositions->release();
                particleSpeeds->release();
                particleColors->release();
                window->clFinish();
            }
            catch (const std::exception &e)
            {
                _log << "cannot launch move_particles kernel: " << e.what() << std::endl;
            }
        }
        drawParticles();
        now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = now - prev;
        if (diff.count())
        {
            window->setTitle(std::string(TITLE).append(" ").append(std::to_string(particles)).append("particles ").append(std::to_string((size_t)(1000 / (diff.count()))).append("fps")));
            /*if (diff.count() < 1000 / FPS_LOCK)
                usleep((1000 / FPS_LOCK - diff.count()) * 1000);*/
        }
        prev = now;
    }
    return (EXIT_SUCCESS);
}
