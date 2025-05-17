#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(const Transform& tranform)
{
	m_position = tranform.m_position;
	m_rotation = tranform.m_rotation;
	m_scale = tranform.m_scale;


	m_dirty = true;
}

Transform& Transform::operator=(const Transform& tranform)
{
	m_position = tranform.m_position;
	m_rotation = tranform.m_rotation;
	m_scale = tranform.m_scale;

	m_dirty = true;
	return *this;
}

void Transform::updateModelMatrix()
{
	m_model = recalcModelMatrix();
	m_dirty = false;
}

void Transform::updateModelMatrix(const glm::mat4& parent)
{
	m_model = parent * recalcModelMatrix();
	m_dirty = false;
}

void Transform::setPosition(glm::vec3 new_position)
{
	m_position = new_position;
	m_dirty = true;
}

void Transform::setRotationEuler(glm::vec3 euler_degrees)
{
	m_rotation = glm::quat(glm::radians(euler_degrees));
	m_dirty = true;
}

void Transform::setRotationQuat(glm::quat& quat)
{
	m_rotation = quat;
	m_dirty = true;
}

void Transform::setScale(glm::vec3 new_scale)
{
	m_scale = new_scale;
	m_dirty = true;
}

glm::vec3 Transform::getPosition() const
{
	return m_position;
}

glm::quat Transform::getRotationQuat() const
{
	return m_rotation;
}

glm::vec3 Transform::getRotationEuler() const
{
	return glm::degrees(glm::eulerAngles(m_rotation));
}

glm::vec3 Transform::getScale() const
{
	return m_scale;
}

bool Transform::isDirty() const
{
	return m_dirty;
}
const glm::mat4& Transform::getModelMatrix() const
{
	return m_model;
}

glm::mat4 Transform::recalcModelMatrix()
{
	glm::mat4 i = glm::mat4(1.0f);

	glm::mat4 S = glm::scale(i, m_scale);
	glm::mat4 R = glm::mat4_cast(m_rotation);
	glm::mat4 T = glm::translate(i, m_position);

	return T * R * S;
}