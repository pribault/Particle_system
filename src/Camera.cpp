#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/rotate_vector.hpp"

#include "Log.h"

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up, const float &fov, const float &aspect, const float &near, const float &far) :
	_pos(pos), _dir(glm::normalize(dir)), _up(glm::normalize(up)), _fov(fov), _aspect(aspect), _near(near), _far(far)
{
	updateView();
	updateProjection();
}

Camera::~Camera(void)
{
}

const glm::mat4	&Camera::getView(void) const
{
	return (_view);
}

const glm::mat4	&Camera::getProjection(void) const
{
	return (_projection);
}

const glm::vec3	&Camera::getPos(void) const
{
	return (_pos);
}

const glm::vec3	&Camera::getDir(void) const
{
	return (_dir);
}

const glm::vec3	&Camera::getUp(void) const
{
	return (_up);
}

const float		&Camera::getFov(void) const
{
	return (_fov);
}

const float		&Camera::getAspect(void) const
{
	return (_aspect);
}

void	Camera::updateView(void)
{
	_view = glm::lookAt(_pos, _pos + _dir, _up);
}

void	Camera::updateProjection(void)
{
	_projection = glm::perspective(_fov, _aspect, _near, _far);
}

void	Camera::moveForward(const float &step)
{
	_pos += _dir * step;
	updateView();
}

void	Camera::moveBackward(const float &step)
{
	_pos -= _dir * step;
	updateView();
}

void	Camera::moveLeft(const float &step)
{
	_pos += glm::rotate(_dir, glm::radians((float)90.0), _up) * step;
	updateView();
}

void	Camera::moveRight(const float &step)
{
	_pos += glm::rotate(_dir, glm::radians((float)-90.0), _up) * step;
	updateView();
}

void	Camera::moveUp(const float &step)
{
	_pos += _up * step;
	updateView();
}

void	Camera::moveDown(const float &step)
{
	_pos -= _up * step;
	updateView();
}

void	Camera::turnUp(const float &angle)
{
	glm::vec3	dir = _dir;
	glm::vec3	up = _up;

	_up = glm::normalize(glm::rotate(up, angle, glm::rotate(dir, glm::radians((float)-90.0), up)));
	_dir = glm::normalize(glm::rotate(dir, angle, glm::rotate(dir, glm::radians((float)-90.0), up)));
	updateView();
}

void	Camera::turnDown(const float &angle)
{
	glm::vec3	dir = _dir;
	glm::vec3	up = _up;

	_up = glm::normalize(glm::rotate(up, -angle, glm::rotate(dir, glm::radians((float)-90.0), up)));
	_dir = glm::normalize(glm::rotate(dir, -angle, glm::rotate(dir, glm::radians((float)-90.0), up)));
	updateView();
}

void	Camera::turnLeft(const float &angle)
{
	_dir = glm::normalize(glm::rotate(_dir, angle, _up));
	updateView();
}

void	Camera::turnRight(const float &angle)
{
	_dir = glm::normalize(glm::rotate(_dir, -angle, _up));
	updateView();
}
