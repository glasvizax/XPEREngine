#include "Model.h"
#include "ShaderProgramManager.h"

Model::Model(const Model& other)
{
	if (&other != this)
	{
		m_meshes_color = other.m_meshes_color;
		m_meshes_d = other.m_meshes_d;
		m_meshes_dn = other.m_meshes_dn;
		m_meshes_dnh = other.m_meshes_dnh;
		m_meshes_ds = other.m_meshes_ds;
		m_meshes_dsn = other.m_meshes_dsn;
		m_meshes_dsnh = other.m_meshes_dsnh;
	}
}

Model& Model::operator=(const Model& other)
{
	if (&other != this)
	{
		m_meshes_color = other.m_meshes_color;
		m_meshes_d = other.m_meshes_d;
		m_meshes_dn = other.m_meshes_dn;
		m_meshes_dnh = other.m_meshes_dnh;
		m_meshes_ds = other.m_meshes_ds;
		m_meshes_dsn = other.m_meshes_dsn;
		m_meshes_dsnh = other.m_meshes_dsnh;
	}
	return *this;
}

Model::Model(Model&& other) noexcept
{
	m_meshes_color = std::move(other.m_meshes_color);
	m_meshes_d = std::move(other.m_meshes_d);
	m_meshes_dn = std::move(other.m_meshes_dn);
	m_meshes_dnh = std::move(other.m_meshes_dnh);
	m_meshes_ds = std::move(other.m_meshes_ds);
	m_meshes_dsn = std::move(other.m_meshes_dsn);
	m_meshes_dsnh = std::move(other.m_meshes_dsnh);
}

Model& Model::operator=(Model&& other) noexcept
{
	if (&other != this)
	{
		m_meshes_color = std::move(other.m_meshes_color);
		m_meshes_d = std::move(other.m_meshes_d);
		m_meshes_dn = std::move(other.m_meshes_dn);
		m_meshes_dnh = std::move(other.m_meshes_dnh);
		m_meshes_ds = std::move(other.m_meshes_ds);
		m_meshes_dsn = std::move(other.m_meshes_dsn);
		m_meshes_dsnh = std::move(other.m_meshes_dsnh);
	}
	return *this;
}

void Model::syncPushMeshColor(ModelEntry<MaterialColor>& entry)
{
	std::lock_guard lk(m_color_mtx);
	m_meshes_color.push_back(entry);
}

void Model::syncPushMeshD(ModelEntry<MaterialD>& entry)
{
	std::lock_guard lk(m_d_mtx);
	m_meshes_d.push_back(entry);
}

void Model::syncPushMeshDN(ModelEntry<MaterialDN>& entry)
{
	std::lock_guard lk(m_dn_mtx);
	m_meshes_dn.push_back(entry);
}

void Model::syncPushMeshDNH(ModelEntry<MaterialDNH>& entry)
{
	std::lock_guard lk(m_dnh_mtx);
	m_meshes_dnh.push_back(entry);
}

void Model::syncPushMeshDS(ModelEntry<MaterialDS>& entry)
{
	std::lock_guard lk(m_ds_mtx);
	m_meshes_ds.push_back(entry);
}

void Model::syncPushMeshDSN(ModelEntry<MaterialDSN>& entry)
{
	std::lock_guard lk(m_dsn_mtx);
	m_meshes_dsn.push_back(entry);
}

void Model::syncPushMeshDSNH(ModelEntry<MaterialDSNH>& entry)
{
	std::lock_guard lk(m_dsnh_mtx);
	m_meshes_dsnh.push_back(entry);
}

void Model::draw(const xm::mat4& model)
{
	for (int i = 0; i < m_meshes_color.size(); ++i)
	{
		m_meshes_color[i].m_material.apply();
		m_meshes_color[i].m_material.m_shader_program->setMat("model", model);
		m_meshes_color[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_d.size(); ++i)
	{
		m_meshes_d[i].m_material.apply();
		m_meshes_d[i].m_material.m_shader_program->setMat("model", model);
		m_meshes_d[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_dn.size(); ++i)
	{
		m_meshes_dn[i].m_material.apply();
		m_meshes_dn[i].m_material.m_shader_program->setMat("model", model);
		m_meshes_dn[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_dnh.size(); ++i)
	{
		m_meshes_dnh[i].m_material.apply();
		m_meshes_dnh[i].m_material.m_shader_program->setMat("model", model);
		m_meshes_dnh[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_ds.size(); ++i)
	{
		m_meshes_ds[i].m_material.apply();
		m_meshes_ds[i].m_material.m_shader_program->setMat("model", model);
		m_meshes_ds[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_dsn.size(); ++i)
	{
		m_meshes_dsn[i].m_material.apply();
		m_meshes_dsn[i].m_material.m_shader_program->setMat("model", model);
		m_meshes_dsn[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_dsnh.size(); ++i)
	{
		m_meshes_dsnh[i].m_material.apply();
		m_meshes_dsnh[i].m_material.m_shader_program->setMat("model", model);
		m_meshes_dsnh[i].m_mesh->draw();
	}
}

void Model::drawCustom(const xm::mat4 model, ShaderProgram* custom_sp)
{
	custom_sp->setMat("model", model);
	for (int i = 0; i < m_meshes_color.size(); ++i)
	{
		m_meshes_color[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_d.size(); ++i)
	{
		m_meshes_d[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_dn.size(); ++i)
	{
		m_meshes_dn[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_dnh.size(); ++i)
	{
		m_meshes_dnh[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_ds.size(); ++i)
	{
		m_meshes_ds[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_dsn.size(); ++i)
	{
		m_meshes_dsn[i].m_mesh->draw();
	}
	for (int i = 0; i < m_meshes_dsnh.size(); ++i)
	{
		m_meshes_dsnh[i].m_mesh->draw();
	}
}
