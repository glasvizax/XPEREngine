#include "Entity.h"

Entity::Entity(Entity&& other) noexcept
{
	m_transform = other.m_transform;
	m_children = std::move(other.m_children);
	m_parent = other.m_parent;
	m_model = std::move(other.m_model);

	for (auto& child : m_children)
	{
		child.m_parent = this;
	}
}

Entity& Entity::operator=(Entity&& other) noexcept
{
	if (&other == this)
	{
		m_transform = other.m_transform;
		m_children = std::move(other.m_children);
		m_parent = other.m_parent;
		m_model = std::move(other.m_model);

		for (auto& child : m_children)
		{
			child.m_parent = this;
		}
	}

	return *this;
}

Entity::Entity(const Model& model)
{
	m_model = model;
}

Entity::Entity(Model&& model)
{
	m_model = std::move(model);
}

Entity::Entity(const Model& model, const Transform& transform)
{
	m_model = model;
	m_transform = transform;
}

Entity::Entity(Model&& model, const Transform& transform)
{
	m_model = std::move(model);
	m_transform = transform;
}

Entity& Entity::addChild()
{
	m_children.emplace_back();
	m_children.back().m_parent = this;
	return m_children.back();
}

Entity& Entity::addChild(const Model& model)
{
	m_children.emplace_back(model);
	m_children.back().m_parent = this;
	return m_children.back();
}

Entity& Entity::addChild(Model&& model)
{
	m_children.emplace_back(std::move(model));
	m_children.back().m_parent = this;
	return m_children.back();
}

Entity& Entity::addChild(const Model& model, const Transform& transform)
{
	m_children.emplace_back(model, transform);
	m_children.back().m_parent = this;
	return m_children.back();
}

Entity& Entity::addChild(Model&& model, const Transform& transform)
{
	m_children.emplace_back(std::move(model), transform);
	m_children.back().m_parent = this;
	return m_children.back();
}

void Entity::update()
{
	if (m_transform.isDirty())
	{
		if (m_parent)
		{
			m_transform.updateModelMatrix(m_parent->getTransform().getModelMatrix());
		}
		else
		{
			m_transform.updateModelMatrix();
		}
		for (auto& e : m_children)
		{
			e.forceUpdateTransform();
		}
		return; // TODO improve
	}

	for (auto& e : m_children)
	{
		e.update();
	}
}

void Entity::draw()
{
	if (m_parent)
	{
		m_model.draw(m_transform.getModelMatrix());
	}

	for (auto& e : m_children)
	{
		e.draw();
	}
}

void Entity::drawCustom(ShaderProgram* custom_sp)
{
	if (m_parent)
	{
		m_model.drawCustom(m_transform.getModelMatrix(), custom_sp);
	}

	for (auto& e : m_children)
	{
		e.drawCustom(custom_sp);
	}
}

void Entity::forceUpdateTransform()
{
	m_transform.updateModelMatrix(m_parent->getTransform().getModelMatrix());
	for (auto& e : m_children)
	{
		e.forceUpdateTransform();
	}
}
