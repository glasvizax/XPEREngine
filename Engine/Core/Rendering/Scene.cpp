#include "Scene.h"

Transform::Transform(const Transform& tranform)
{
	m_position = tranform.m_position;
	m_rotation = tranform.m_rotation;
	m_scale = tranform.m_scale;

	m_dirty = true;
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

void Transform::setRotation(glm::vec3 new_rotation)
{
	m_rotation = new_rotation;
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

glm::vec3 Transform::getRotation() const
{
	return m_rotation;
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

	glm::mat4 rot_x = glm::rotate(i, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rot_y = glm::rotate(i, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rot_z = glm::rotate(i, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 S = glm::scale(i, m_scale);
	glm::mat4 R = rot_y * rot_x * rot_z;
	glm::mat4 T = glm::translate(i, m_position);

	return T * R * S;
}

Entity::Entity(const Model& model)
{
	m_model = model;
}

Entity::Entity(const Model& model, const Transform& transform)
{
	m_model = model;
	m_transform = transform;
}

void Entity::addChild(const Model& model)
{
	m_children.emplace_back(model);
	m_children.back().m_parent = this;
}

void Entity::addChild(const Model& model, const Transform& transform)
{
	m_children.emplace_back(model, transform);
	m_children.back().m_parent = this;
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
		for (int i = 0; i < m_model.color_entries.size(); ++i)
		{
			m_model.color_entries[i].material->apply();
			m_model.color_entries[i].material->m_shader_program->use();
			m_model.color_entries[i].material->m_shader_program->setMat("model", m_transform.getModelMatrix());
			m_model.color_entries[i].mesh->draw();
		}
	}

	for (auto& e : m_children)
	{
		e.draw();
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
