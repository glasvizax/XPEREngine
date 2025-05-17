#pragma once
#include <vector>
#include <mutex>
#include <thread>

#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Material.h"

template <typename MaterialClass>
struct ModelEntry
{
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

	void syncPushColorEntry(ModelEntry<MaterialColor>& entry);

	void syncPushDiffEntry(ModelEntry<MaterialDiff>& entry);

	void syncPushDiffSpecEntry(ModelEntry<MaterialDiffSpec>& entry);

	void syncPushDiffSpecNormEntry(ModelEntry<MaterialDiffSpecNorm>& entry);

	void syncPushDiffSpecNormHeightEntry(ModelEntry<MaterialDiffSpecNormHeight>& entry);

	void draw(const glm::mat4& model);

private:
	std::vector<ModelEntry<MaterialColor>>	  m_color_meshes;
	std::vector<ModelEntry<MaterialDiff>>	  m_diff_meshes;
	std::vector<ModelEntry<MaterialDiffSpec>> m_diff_spec_meshes;
	std::vector<ModelEntry<MaterialDiffSpecNorm>> m_diff_spec_norm_meshes;
	std::vector<ModelEntry<MaterialDiffSpecNormHeight>> m_diff_spec_norm_height_meshes;

	std::mutex m_color_mtx;
	std::mutex m_diff_mtx;
	std::mutex m_diff_spec_mtx;
	std::mutex m_diff_spec_norm_mtx;
	std::mutex m_diff_spec_norm_height_mtx;
};