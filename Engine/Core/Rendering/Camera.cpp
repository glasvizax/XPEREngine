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
}

void Camera::rotateYaw(float degrees)
{
	m_yaw += degrees;
}

void Camera::move(glm::vec3 where)
{
	m_position += where;
}

void Camera::setPosition(glm::vec3 new_position)
{
	m_position = new_position;
}

void Camera::setZoom(float new_zoom)
{
	m_zoom = new_zoom;
}

void Camera::setFovVertical(float fov)
{
	m_fov = fov;
}

void Camera::setAspectRatio(float aspect_ratio)
{
	m_aspect_ratio = aspect_ratio;
}

void Camera::setNearPlane(float near)
{
	m_near = near;
}

void Camera::setFarPlane(float far)
{
	m_far = far;
}

float Camera::getZoom()
{
	return m_zoom;
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

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_look_dir, m_up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(m_fov, m_aspect_ratio, m_near, m_far);
}

void Camera::updateVectors()
{
	float cos_yaw = cos(glm::radians(m_yaw));
	float cos_pitch = cos(glm::radians(m_pitch));

	float sin_yaw = glm::sqrt((1 - glm::pow(cos_yaw, 2)));
	float sin_pitch = glm::sqrt((1 - glm::pow(cos_pitch, 2)));

	m_look_dir.x = cos_yaw * cos_pitch;
	m_look_dir.y = sin_pitch;
	m_look_dir.z = sin_yaw * cos_pitch;

	m_look_dir = glm::normalize(m_look_dir);

	m_right = glm::normalize(glm::cross(m_look_dir, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_look_dir));
}
