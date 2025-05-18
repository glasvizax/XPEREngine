#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"
#include "Transform.h"

class Entity
{
	friend class ResourceManager;
	friend class RenderSystem;
	
public:
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	Entity(Entity&& other) noexcept;
	Entity& operator=(Entity&& other) noexcept;

	Entity() = default;
	Entity(const Model& model);
	Entity(Model&& model);
	Entity(const Model& model, const Transform& transform);
	Entity(Model&& model, const Transform& transform);
	
	Entity& addChild();
	Entity& addChild(const Model& model);
	Entity& addChild(Model&& model);

	Entity& addChild(const Model& model, const Transform& transform);
	Entity& addChild(Model&& model, const Transform& transform);

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
