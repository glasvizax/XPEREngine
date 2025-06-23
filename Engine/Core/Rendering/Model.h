#pragma once
#include <vector>
#include <mutex>
#include <thread>

#include "Mesh.h"
#include "Material.h"

#include "xm/xm.h"

template <typename MaterialClass>
struct ModelEntry
{
	ModelEntry() = default;
	Mesh*		  m_mesh;
	MaterialClass m_material;
};

struct Model
{
	Model() = default;

	Model(const Model& other);

	Model& operator=(const Model& other);

	Model(Model&& other) noexcept;

	Model& operator=(Model&& other) noexcept;

	void syncPushMeshColor(ModelEntry<MaterialColor>& entry);

	void syncPushMeshD(ModelEntry<MaterialD>& entry);

	void syncPushMeshDN(ModelEntry<MaterialDN>& entry);

	void syncPushMeshDNH(ModelEntry<MaterialDNH>& entry);

	void syncPushMeshDS(ModelEntry<MaterialDS>& entry);

	void syncPushMeshDSN(ModelEntry<MaterialDSN>& entry);

	void syncPushMeshDSNH(ModelEntry<MaterialDSNH>& entry);

	void draw(const xm::mat4& model);

	void drawCustom(const xm::mat4 model, ShaderProgram* custom_sp);

	std::vector<ModelEntry<MaterialColor>> m_meshes_color;
	std::vector<ModelEntry<MaterialD>>	   m_meshes_d;
	std::vector<ModelEntry<MaterialDN>>	   m_meshes_dn;
	std::vector<ModelEntry<MaterialDNH>>   m_meshes_dnh;
	std::vector<ModelEntry<MaterialDS>>	   m_meshes_ds;
	std::vector<ModelEntry<MaterialDSN>>   m_meshes_dsn;
	std::vector<ModelEntry<MaterialDSNH>>  m_meshes_dsnh;

private:
	std::mutex m_color_mtx;
	std::mutex m_d_mtx;
	std::mutex m_dn_mtx;
	std::mutex m_dnh_mtx;
	std::mutex m_ds_mtx;
	std::mutex m_dsn_mtx;
	std::mutex m_dsnh_mtx;
};