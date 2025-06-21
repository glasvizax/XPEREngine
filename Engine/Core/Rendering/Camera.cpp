#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Debug.h"
#include "xm/math_helpers.h"
#include "xm/quaternion.h"

void Camera::rotateX(float degrees)
{
	m_rotation.x += degrees;

	if (m_rotation.x > 89.0f)
	{
		m_rotation.x = 89.0f;
	}
	if (m_rotation.x < -89.0f)
	{
		m_rotation.x = -89.0f;
	}

	updateVectors();
	m_view_dirty = true;
}

void Camera::rotateY(float degrees)
{
	m_rotation.y += degrees;
	updateVectors();
	m_view_dirty = true;
}

void Camera::move(xm::vec3 where)
{
	m_position += where;
	m_view_dirty = true;
}

void Camera::setPosition(xm::vec3 new_position)
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

xm::vec3 Camera::getLookVector()
{
	return m_look_dir;
}

xm::vec3 Camera::getRightVector()
{
	return m_right;
}

xm::vec3 Camera::getUpVector()
{
	return m_up;
}

xm::vec3 Camera::getPosition()
{
	return m_position;
}

xm::mat4 Camera::getViewMatrix()
{
	if (m_view_dirty)
	{
		std::tie(m_view, std::ignore, std::ignore) = xm::lookAt(m_position, m_look_dir, m_up);
		m_view_dirty = false;
	}

	return m_view;
}

xm::matrix<4, float> Camera::getProjectionMatrix()
{
	if (m_proj_dirty)
	{
		m_projection = xm::perspective(xm::to_radians(m_fov), m_aspect_ratio, m_near, m_far);
		m_proj_dirty = false;
	}

	return m_projection;
}

bool Camera::isViewDirty()
{
	return m_view_dirty;
}

bool Camera::isProjectionDirty()
{
	return m_proj_dirty;
}

void Camera::updateVectors()
{
	xm::quat quat_x = xm::quat_from_euler_x(xm::to_radians(m_rotation.x));
	xm::quat quat_y = xm::quat_from_euler_y(xm::to_radians(m_rotation.y));
	xm::quat res_quat = quat_y * quat_x;
	xm::mat3 res_coords = xm::mat3_cast(res_quat);

	m_right = res_coords.a;
	// m_up = res_coords.b;
	m_look_dir = -res_coords.c;
}
