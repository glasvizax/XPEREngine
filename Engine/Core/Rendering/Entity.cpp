#include "Entity.h"

Entity::Entity(Entity&& other) noexcept
{
	this->m_transform = other.m_transform;
	this->m_children = std::move(other.m_children);
	this->m_parent = other.m_parent;
	this->m_model = other.m_model;
}

Entity& Entity::operator=(Entity&& other) noexcept
{
	this->m_transform = other.m_transform;
	this->m_children = std::move(other.m_children);
	this->m_parent = other.m_parent;
	this->m_model = other.m_model;

	return *this;
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

Entity& Entity::addChild(const Model& model)
{
	m_children.emplace_back(model);
	m_children.back().m_parent = this;
	return m_children.back();
}

Entity& Entity::addChild(const Model& model, const Transform& transform)
{
	m_children.emplace_back(model, transform);
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
		for (int i = 0; i < m_model.m_color_meshes.size(); ++i)
		{
			m_model.m_color_meshes[i].m_material.apply();
			m_model.m_color_meshes[i].m_material.m_shader_program->setMat("model", m_transform.getModelMatrix());
			m_model.m_color_meshes[i].m_mesh->draw();
		}
		for (int i = 0; i < m_model.m_diff_meshes.size(); ++i)
		{
			m_model.m_diff_meshes[i].m_material.apply();
			m_model.m_diff_meshes[i].m_material.m_shader_program->setMat("model", m_transform.getModelMatrix());
			m_model.m_diff_meshes[i].m_mesh->draw();
		}
		for (int i = 0; i < m_model.m_diff_spec_meshes.size(); ++i)
		{
			m_model.m_diff_spec_meshes[i].m_material.apply();
			m_model.m_diff_spec_meshes[i].m_material.m_shader_program->setMat("model", m_transform.getModelMatrix());
			m_model.m_diff_spec_meshes[i].m_mesh->draw();
		}
		for (int i = 0; i < m_model.m_diff_spec_norm_meshes.size(); ++i)
		{
			m_model.m_diff_spec_norm_meshes[i].m_material.apply();
			m_model.m_diff_spec_norm_meshes[i].m_material.m_shader_program->setMat("model", m_transform.getModelMatrix());
			m_model.m_diff_spec_norm_meshes[i].m_mesh->draw();
		}
		for (int i = 0; i < m_model.m_diff_spec_norm_height_meshes.size(); ++i)
		{
			m_model.m_diff_spec_norm_height_meshes[i].m_material.apply();
			m_model.m_diff_spec_norm_height_meshes[i].m_material.m_shader_program->setMat("model", m_transform.getModelMatrix());
			m_model.m_diff_spec_norm_height_meshes[i].m_mesh->draw();
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
