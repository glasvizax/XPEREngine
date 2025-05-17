#pragma once
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "Entity.h"

template <typename MaterialClass>
struct ModelEntry
{
	Mesh*		  m_mesh;
	MaterialClass m_material;
};

struct Model
{
	std::vector<ModelEntry<MaterialColor>>	  m_color_meshes;
	std::vector<ModelEntry<MaterialDiff>>	  m_diff_meshes;
	std::vector<ModelEntry<MaterialDiffSpec>> m_diff_spec_meshes;
	std::vector<ModelEntry<MaterialDiffSpecNorm>> m_diff_spec_norm_meshes;
	std::vector<ModelEntry<MaterialDiffSpecNormHeight>> m_diff_spec_norm_height_meshes;
};