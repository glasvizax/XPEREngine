#pragma once
#include <glm/glm.hpp>

struct Transform
{

public:
	Transform() = default;

	Transform(const Transform& tranform);

public:
	void updateModelMatrix();

	void updateModelMatrix(const glm::mat4& parent);

	void setPosition(glm::vec3 new_position);

	void setRotation(glm::vec3 new_rotation);

	void setScale(glm::vec3 new_scale);

	glm::vec3 getPosition() const;

	glm::vec3 getRotation() const;

	glm::vec3 getScale() const;

	bool isDirty() const;

	const glm::mat4& getModelMatrix() const;

private:
	glm::mat4 recalcModelMatrix();

private:
	glm::mat4 m_model = glm::mat4(1.0f);

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);

	bool m_dirty = false;
};
