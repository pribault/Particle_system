#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class	Camera
{
	public:
		Camera(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fov, const float &aspect, const float &near, const float &far);
		~Camera(void);

		const glm::mat4	&getView(void) const;
		const glm::mat4	&getProjection(void) const;
		const glm::vec3	&getPos(void) const;
		const glm::vec3	&getDir(void) const;
		const glm::vec3	&getUp(void) const;
		const float		&getFov(void) const;
		const float		&getAspect(void) const;

		void	moveForward(const float &step);
		void	moveBackward(const float &step);
		void	moveLeft(const float &step);
		void	moveRight(const float &step);
		void	moveUp(const float &step);
		void	moveDown(const float &step);

		void	turnUp(const float &angle);
		void	turnDown(const float &angle);
		void	turnLeft(const float &angle);
		void	turnRight(const float &angle);
	private:
		void	updateView(void);
		void	updateProjection(void);

		glm::mat4	_view;
		glm::mat4	_projection;
		glm::vec3	_pos;
		glm::vec3	_dir;
		glm::vec3	_up;
		float		_fov;
		float		_aspect;
		float		_near;
		float		_far;
};
