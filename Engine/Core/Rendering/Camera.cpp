#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::rotatePitch(float degrees)
{
	m_pitch += degrees;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	updateVectors();
	m_view_dirty = true;
}

void Camera::rotateYaw(float degrees)
{
	m_yaw += degrees;
	updateVectors();
	m_view_dirty = true;
}

void Camera::move(glm::vec3 where)
{
	m_position += where;
	m_view_dirty = true;
}

void Camera::setPosition(glm::vec3 new_position)
{
	m_position = new_position;
	m_view_dirty = true;
}

void Camera::setFovVertical(float fov)
{
	m_fov = fov;
	m_proj_dirty = true;
}

void Camera::setAspectRatio(float aspect_ratio)
{
	m_aspect_ratio = aspect_ratio;
	m_proj_dirty = true;
}

void Camera::setNearPlane(float _near)
{
	m_near = _near;
	m_proj_dirty = true;
}

void Camera::setFarPlane(float _far)
{
	m_far = _far;
	m_proj_dirty = true;
}

float Camera::getFovVertical()
{
	return m_fov;
}

float Camera::getAspectRatio()
{
	return m_aspect_ratio;
}

float Camera::getNearPlane()
{
	return m_near;
}

float Camera::getFarPlane()
{
	return m_far;
}

glm::vec3 Camera::getLookVector()
{
	return m_look_dir;
}

glm::vec3 Camera::getRightVector()
{
	return m_right;
}

glm::vec3 Camera::getUpVector()
{
	return m_up;
}

glm::mat4 Camera::getViewMatrix()
{
	if (m_view_dirty)
	{
		view = glm::lookAt(m_position, m_position + m_look_dir, m_up);
		m_view_dirty = false;
	}
	
	return view;
}

glm::mat4 Camera::getProjectionMatrix()
{
	if (m_proj_dirty)
	{
		projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
		m_proj_dirty = false;
	}

	return projection;
}

void Camera::updateVectors()
{
	float cos_yaw = glm::cos(glm::radians(m_yaw));
	float cos_pitch = glm::cos(glm::radians(m_pitch));

	//float sin_yaw = glm::sqrt((1 - glm::pow(cos_yaw, 2)));
	//float sin_pitch = glm::sqrt((1 - glm::pow(cos_pitch, 2)));

	float sin_yaw = glm::sin(glm::radians(m_yaw));
	float sin_pitch = glm::sin(glm::radians(m_pitch));

	m_look_dir.x = cos_yaw * cos_pitch;
	m_look_dir.y = sin_pitch;
	m_look_dir.z = sin_yaw * cos_pitch;

	m_look_dir = glm::normalize(m_look_dir);

	m_right = glm::normalize(glm::cross(m_look_dir, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_look_dir));
}
