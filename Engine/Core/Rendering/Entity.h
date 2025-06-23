#pragma once

#include <vector>

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

	Entity* getParent();

	Transform& getTransform();

	void draw();

	void drawCustom(ShaderProgram* custom_sp);

private:
	void forceUpdateTransform();

private:
	Transform m_transform;

	Model m_model;

	std::vector<Entity> m_children;
	Entity*				m_parent = nullptr;
};
