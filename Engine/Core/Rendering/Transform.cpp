#include "Transform.h"
#include "xm/math_helpers.h"

Transform::Transform(const Transform& tranform)
{
	m_position = tranform.m_position;
	m_rotation_eul = tranform.m_rotation_eul;
	m_rotation_quat = tranform.m_rotation_quat;
	m_scale = tranform.m_scale;

	m_dirty = true;
}

Transform& Transform::operator=(const Transform& tranform)
{
	m_position = tranform.m_position;
	m_rotation_eul = tranform.m_rotation_eul;
	m_rotation_quat = tranform.m_rotation_quat;
	m_scale = tranform.m_scale;

	m_dirty = true;
	return *this;
}

Transform& Transform::updateModelMatrix()
{
	m_model = recalcModelMatrix();
	m_dirty = false;
	return *this;
}

Transform& Transform::updateModelMatrix(const xm::mat4& parent)
{
	m_model = parent * recalcModelMatrix();
	m_dirty = false;
	return *this;
}

Transform& Transform::setPosition(xm::vec3 new_position)
{
	m_position = new_position;
	m_dirty = true;
	return *this;
}

Transform& Transform::setRotationEuler(xm::vec3 euler_degrees)
{
	m_rotation_eul = euler_degrees;

	m_rotation_quat = xm::quat_from_euler_xyz(m_rotation_eul);

	m_dirty = true;
	return *this;
}

Transform& Transform::setRotationQuat(xm::quat& quat)
{
	m_rotation_quat = quat;
	m_dirty = true;
	return *this;
}

Transform& Transform::setScale(xm::vec3 new_scale)
{
	m_scale = new_scale;
	m_dirty = true;
	return *this;
}

xm::vec3 Transform::getPosition() const
{
	return m_position;
}

xm::quat Transform::getRotationQuat() const
{
	return m_rotation_quat;
}

xm::vec3 Transform::getRotationEuler() const
{
	return m_rotation_eul;
}

xm::vec3 Transform::getScale() const
{
	return m_scale;
}

bool Transform::isDirty() const
{
	return m_dirty;
}
const xm::mat4& Transform::getModelMatrix() const
{
	return m_model;
}

xm::mat4 Transform::recalcModelMatrix()
{
	xm::mat4 i = xm::mat4(1.0f);

	xm::mat4 S = xm::scale(i, m_scale);
	xm::mat4 R = xm::mat4_cast(m_rotation_quat);
	xm::mat4 T = xm::translate(i, m_position);

	return T * R * S;
}