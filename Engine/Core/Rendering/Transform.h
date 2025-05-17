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
	void updateModelMatrix();

	void updateModelMatrix(const glm::mat4& parent);

	void setPosition(glm::vec3 new_position);

	void setRotationEuler(glm::vec3 euler_degrees);

	void setRotationQuat(glm::quat& quat);

	void setScale(glm::vec3 new_scale);

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
