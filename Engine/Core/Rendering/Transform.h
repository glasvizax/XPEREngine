#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

	Transform& updateModelMatrix(const glm::mat4& parent);

	Transform& setPosition(glm::vec3 new_position);

	Transform& setRotationEuler(glm::vec3 euler_degrees);

	Transform& setRotationQuat(glm::quat& quat);

	Transform& setScale(glm::vec3 new_scale);

	glm::vec3 getPosition() const;

	glm::quat getRotationQuat() const;

	glm::vec3 getRotationEuler() const;

	glm::vec3 getScale() const;

	bool isDirty() const;

	const glm::mat4& getModelMatrix() const;

private:
	glm::mat4 recalcModelMatrix();

private:
	glm::mat4 m_model = glm::mat4(1.0f);

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);

	bool m_dirty = false;
};
