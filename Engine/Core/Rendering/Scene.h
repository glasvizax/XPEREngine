#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

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

class Entity
{
	friend class RenderSystem;
	Entity() = default;

public:
	Entity(const Model& model);

	Entity(const Model& model, const Transform& transform);

	void addChild(const Model& model);

	void addChild(const Model& model, const Transform& transform);

	void update();

	Entity* getParent()
	{
		return m_parent;
	}

	Transform& getTransform()
	{
		return m_transform;
	}

	void draw();

private:
	void forceUpdateTransform();

private:
	Transform m_transform;

	Model m_model;

	std::vector<Entity> m_children;
	Entity*				m_parent = nullptr;
};
