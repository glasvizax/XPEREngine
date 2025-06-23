#pragma once

#include "xm/xm.h"

struct Transform
{

public:
	Transform() = default;

	Transform(const Transform& tranform);
	Transform& operator=(const Transform& tranform);

	Transform(Transform&& tranform) noexcept = delete;
	Transform& operator=(Transform&& tranform) noexcept = delete;

public:
	Transform& updateModelMatrix();

	Transform& updateModelMatrix(const xm::mat4& parent);

	Transform& setPosition(xm::vec3 new_position);

	Transform& setRotationEuler(xm::vec3 euler_degrees);

	Transform& setRotationQuat(xm::quat& quat);

	Transform& setScale(xm::vec3 new_scale);

	xm::vec3 getPosition() const;

	xm::quat getRotationQuat() const;

	xm::vec3 getRotationEuler() const;

	xm::vec3 getScale() const;

	bool isDirty() const;

	const xm::mat4& getModelMatrix() const;

private:
	xm::mat4 recalcModelMatrix();

private:
	xm::mat4 m_model = xm::mat4(1.0f);

	xm::vec3 m_rotation_eul;
	xm::vec3 m_position = xm::vec3(0.0f);
	xm::quat m_rotation_quat = xm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	xm::vec3 m_scale = xm::vec3(1.0f);

	bool m_dirty = false;
};
