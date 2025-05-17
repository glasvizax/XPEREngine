#include "Model.h"

Model::Model(const Model& other)
{
	if (&other != this)
	{
		m_color_meshes = other.m_color_meshes;
		m_diff_meshes = other.m_diff_meshes;
		m_diff_spec_meshes = other.m_diff_spec_meshes;
		m_diff_spec_norm_meshes = other.m_diff_spec_norm_meshes;
		m_diff_spec_norm_height_meshes = other.m_diff_spec_norm_height_meshes;
	}
}

Model& Model::operator=(const Model& other)
{
	if (&other != this)
	{
		m_color_meshes = other.m_color_meshes;
		m_diff_meshes = other.m_diff_meshes;
		m_diff_spec_meshes = other.m_diff_spec_meshes;
		m_diff_spec_norm_meshes = other.m_diff_spec_norm_meshes;
		m_diff_spec_norm_height_meshes = other.m_diff_spec_norm_height_meshes;
	}
	return *this;
}

Model::Model(Model&& other) noexcept
{
	if (&other != this)
	{
		m_color_meshes = std::move(other.m_color_meshes);
		m_diff_meshes = std::move(other.m_diff_meshes);
		m_diff_spec_meshes = std::move(other.m_diff_spec_meshes);
		m_diff_spec_norm_meshes = std::move(other.m_diff_spec_norm_meshes);
		m_diff_spec_norm_height_meshes = std::move(other.m_diff_spec_norm_height_meshes);
	}
}

Model& Model::operator=(Model&& other) noexcept
{
	if (&other != this)
	{
		m_color_meshes = std::move(other.m_color_meshes);
		m_diff_meshes = std::move(other.m_diff_meshes);
		m_diff_spec_meshes = std::move(other.m_diff_spec_meshes);
		m_diff_spec_norm_meshes = std::move(other.m_diff_spec_norm_meshes);
		m_diff_spec_norm_height_meshes = std::move(other.m_diff_spec_norm_height_meshes);
	}
	return *this;
}

void Model::syncPushColorEntry(ModelEntry<MaterialColor>& entry)
{
	std::lock_guard lk(m_color_mtx);
	m_color_meshes.push_back(entry);
}

void Model::syncPushDiffEntry(ModelEntry<MaterialDiff>& entry)
{
	std::lock_guard lk(m_diff_mtx);
	m_diff_meshes.push_back(entry);
}

void Model::syncPushDiffSpecEntry(ModelEntry<MaterialDiffSpec>& entry)
{
	std::lock_guard lk(m_diff_spec_mtx);
	m_diff_spec_meshes.push_back(entry);
}

void Model::syncPushDiffSpecNormEntry(ModelEntry<MaterialDiffSpecNorm>& entry)
{
	std::lock_guard lk(m_diff_spec_norm_height_mtx);
	m_diff_spec_norm_meshes.push_back(entry);
}

void Model::syncPushDiffSpecNormHeightEntry(ModelEntry<MaterialDiffSpecNormHeight>& entry)
{
	std::lock_guard lk(m_diff_spec_norm_height_mtx);
	m_diff_spec_norm_height_meshes.push_back(entry);
}

void Model::draw(const glm::mat4& model)
{
	for (int i = 0; i < m_color_meshes.size(); ++i)
	{
		m_color_meshes[i].m_material.apply();
		m_color_meshes[i].m_material.m_shader_program->setMat("model", model);
		m_color_meshes[i].m_mesh->draw();
	}
	for (int i = 0; i < m_diff_meshes.size(); ++i)
	{
		m_diff_meshes[i].m_material.apply();
		m_diff_meshes[i].m_material.m_shader_program->setMat("model", model);
		m_diff_meshes[i].m_mesh->draw();
	}
	for (int i = 0; i < m_diff_spec_meshes.size(); ++i)
	{
		m_diff_spec_meshes[i].m_material.apply();
		m_diff_spec_meshes[i].m_material.m_shader_program->setMat("model", model);
		m_diff_spec_meshes[i].m_mesh->draw();
	}
	for (int i = 0; i < m_diff_spec_norm_meshes.size(); ++i)
	{
		m_diff_spec_norm_meshes[i].m_material.apply();
		m_diff_spec_norm_meshes[i].m_material.m_shader_program->setMat("model", model);
		m_diff_spec_norm_meshes[i].m_mesh->draw();
	}
	for (int i = 0; i < m_diff_spec_norm_height_meshes.size(); ++i)
	{
		m_diff_spec_norm_height_meshes[i].m_material.apply();
		m_diff_spec_norm_height_meshes[i].m_material.m_shader_program->setMat("model", model);
		m_diff_spec_norm_height_meshes[i].m_mesh->draw();
	}
}