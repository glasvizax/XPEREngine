#include "ResourceManager.h"

#include <fstream>
#include <thread>
#include <algorithm>
#include <mutex>
#include <execution>
#include <stb_image.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

#define ASSIMP_ENABLE_THREAD_CHECKS 1
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderProgram.h"
#include "Debug.h"
#include "Texture.h"
#include "Entity.h"
#include "Defines.h"
#include "Utils.h"

#include "Engine.h"
#include "RenderSystem.h"

#include "xm/xm.h"

using namespace std::string_literals;

bool ResourceManager::init()
{
	m_current_path = std::filesystem::current_path();
	m_textures_path.append("content").append("textures");
	return true;
}

void ResourceManager::destroy()
{
}

bool ResourceManager::initLoadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, ShaderProgram& shader_program)
{
	std::string			  vertex_src, fragment_src;
	std::filesystem::path current = m_current_path;
	if (!readFile(current.append(vertex_name), vertex_src))
	{
		return false;
	}

	current = m_current_path;
	if (!readFile(current.append(fragment_name), fragment_src))
	{
		return false;
	}

	return shader_program.init(vertex_src.c_str(), fragment_src.c_str());
}

bool ResourceManager::initLoadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, const std::string& geometry_name, ShaderProgram& shader_program)
{
	std::string vertex_src, fragment_src, geometry_src;

	std::filesystem::path current = m_current_path;
	if (!readFile(current.append(vertex_name), vertex_src))
	{
		return false;
	}
	current = m_current_path;

	if (!readFile(current.append(fragment_name), fragment_src))
	{
		return false;
	}

	current = m_current_path;
	if (!readFile(current.append(geometry_name), geometry_src))
	{
		return false;
	}

	return shader_program.init(vertex_src.c_str(), fragment_src.c_str(), geometry_src.c_str());
}

bool ResourceManager::initLoadTexture(const std::string& name, Texture& texture, bool generate_mipmap)
{
	int width, height, channels_num;

	uchar* data = stbi_load(name.c_str(), &width, &height, &channels_num, 0);
	if (!data)
	{
		LOG_ERROR_F("couldn't load file [%s] reason [%s]", name.c_str(), stbi_failure_reason());
		return false;
	}

	GLint format = GL_RGB;
	GLint internal_format = GL_RGB8;
	switch (channels_num)
	{
		case (1):
			format = GL_RED;
			internal_format = GL_R8;
			channels_num = 1;
			break;
		case (2):
			format = GL_RG;
			internal_format = GL_RG8;
			channels_num = 2;
			break;
		case (4):
			format = GL_RGBA;
			internal_format = GL_RGBA8;
			channels_num = 4;
			break;
		default:
			break;
	}
	texture.init(width, height, internal_format, channels_num, generate_mipmap);
	texture.loadData(GL_UNSIGNED_BYTE, format, data);

	stbi_image_free(data);
	LOG_INFO_F("loaded texture [%s]", name.c_str());
	return true;
}

bool ResourceManager::initLoadCubemap(const std::vector<std::string>& face_files, Cubemap& cubemap, bool generate_mipmap)
{
	if (face_files.size() != 6)
	{
		LOG_ERROR_F("initLoadCubemap: expected 6 faces, got %zu", face_files.size());
		return false;
	}

	int width = 0, height = 0, channels = 0;

	std::vector<uchar*> face_data(6);

	for (size_t i = 0; i < 6; ++i)
	{
		face_data[i] = stbi_load(face_files[i].c_str(), &width, &height, &channels, 0);
		if (!face_data[i])
		{
			LOG_ERROR_F("couldn't load cubemap face [%s] reason [%s]", face_files[i].c_str(), stbi_failure_reason());
			for (size_t j = 0; j < i; ++j)
			{
				stbi_image_free(face_data[j]);
			}
			return false;
		}
	}

	GLenum format = GL_RGB;
	GLint  internal_format = GL_RGB8;
	switch (channels)
	{
		case 1:
			format = GL_RED;
			internal_format = GL_R8;
			break;
		case 2:
			format = GL_RG;
			internal_format = GL_RG8;
			break;
		case 3:
			format = GL_RGB;
			internal_format = GL_RGB8;
			break;
		case 4:
			format = GL_RGBA;
			internal_format = GL_RGBA8;
			break;
		default:
			break;
	}

	cubemap.init(width, internal_format, channels, generate_mipmap);

	xm::vec2 sf(0.0f, 0.0f), ef(1.0f, 1.0f);
	for (GLuint face = 0; face < 6; ++face)
	{
		cubemap.loadFaceData(face, GL_UNSIGNED_BYTE, format, face_data[face], sf, ef);
	}

	for (auto ptr : face_data)
	{
		stbi_image_free(ptr);
	}

	LOG_INFO_F("loaded cubemap [%zu faces]", face_files.size());

	for (auto& n : face_files)
	{
		LOG_INFO_F("loaded cubemap face texture [%s]", n.c_str());
	}

	return true;
}

bool ResourceManager::readFile(const std::filesystem::path& path, std::string& content)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		LOG_ERROR_F("couldn't open file %s", path.generic_string().c_str());
		return false;
	}

	content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	return true;
}

std::vector<fs::path> ResourceManager::findFiles(const std::string& filename)
{
	std::vector<fs::path> res;
	for (const auto& entry : fs::recursive_directory_iterator(m_current_path))
	{
		if (entry.is_regular_file() && entry.path().filename() == filename)
		{
			res.push_back(fs::relative(entry.path(), m_current_path));
		}
	}
	return res;
}

bool _normalmap; // EVIL CRUTCH

bool ResourceManager::loadModel(const std::string path, Entity& root_entity, bool normalmap, bool flip_uv)
{
	Assimp::Importer importer;

	_normalmap = normalmap;
	std::filesystem::path current_path = m_current_path;
	current_path.append(path);

	const aiScene* scene = importer.ReadFile(current_path.generic_string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | (flip_uv ? aiProcess_FlipUVs : 0) | (_normalmap ? aiProcess_CalcTangentSpace : 0));
	if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
	{
		LOG_ERROR_F("couldn't load file [%s] : assimp info : [%s]", path.c_str(), importer.GetErrorString());
		return false;
	}
	processNode(scene->mRootNode, scene, &root_entity);
}

void ResourceManager::processNode(aiNode* node, const aiScene* scene, Entity* parent)
{
	parent->m_children.reserve(parent->m_children.capacity() + node->mNumChildren);
	Entity* next_parent = parent;
	if (node->mNumMeshes > 0)
	{
		glm::vec3 scale(1.0f), translation(0.0f), _skew;
		glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4 _perspective;

		glm::mat4 transform_matrix = assimpToGLMMat4(node->mTransformation);
		if (glm::determinant(transform_matrix) < 0.001f)
		{
			LOG_WARNING_S("Invalid node transformation matrix");
			transform_matrix = glm::mat4(1.0f);
		}
		else
		{
			glm::decompose(transform_matrix, scale, rotation, translation, _skew, _perspective);
		}

		Transform transform;
		transform.setPosition(xm::vec3(translation.x, translation.y, translation.z));
		transform.setRotationQuat(xm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
		transform.setScale(xm::vec3(scale.x, scale.y, scale.z));
		Model model;
		std::for_each(std::execution::seq,
			node->mMeshes,
			node->mMeshes + node->mNumMeshes,
			[&](unsigned int mesh_index) {
				aiMesh* mesh = scene->mMeshes[mesh_index];
				processMesh(mesh, scene, model);
			});

		next_parent = &parent->addChild(model, transform);
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene, next_parent);
	}
}

void ResourceManager::processMesh(aiMesh* mesh, const aiScene* scene, Model& model)
{
	std::vector<uint> indices;
	indices.resize(mesh->mNumFaces * 3);

	uint  num_vertices = mesh->mNumVertices;
	uint  num_faces = mesh->mNumFaces;
	Mesh* final_mesh;

	uint counter = 0;
	for (uint i = 0; i < num_faces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < 3; ++j)
		{
			indices[counter] = face.mIndices[j];
			counter++;
		}
	}

	aiMaterial*			  material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture*> normal;
	if (_normalmap)
	{
		normal = loadMaterialTexture(material, aiTextureType::aiTextureType_HEIGHT, 1); // TODO - TO NORMALS
	}

	if (normal.size() > 0)
	{
		std::vector<VertexTB> vertices;
		vertices.resize(num_vertices);

		for (uint i = 0; i < num_vertices; ++i)
		{
			vertices[i].m_position = xm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertices[i].m_uv.x = mesh->mTextureCoords[0][i].x;
			vertices[i].m_uv.y = mesh->mTextureCoords[0][i].y;
			vertices[i].m_normal = xm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			vertices[i].m_tangent = xm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertices[i].m_bitangent = xm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		final_mesh = syncEmplaceModelMesh(vertices, indices);
	}
	else
	{
		std::vector<Vertex> vertices;
		vertices.resize(num_vertices);
		if (mesh->HasTextureCoords(0))
		{
			for (uint i = 0; i < num_vertices; ++i)
			{
				vertices[i].m_position = xm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertices[i].m_uv.x = mesh->mTextureCoords[0][i].x;
				vertices[i].m_uv.y = mesh->mTextureCoords[0][i].y;
				vertices[i].m_normal = xm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
		}
		else
		{
			for (uint i = 0; i < num_vertices; ++i)
			{
				vertices[i].m_position = xm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertices[i].m_normal = xm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
		}
		final_mesh = syncEmplaceModelMesh(vertices, indices);
	}

	processMaterial(final_mesh, material, model);
}

void ResourceManager::processMaterial(Mesh* mesh, aiMaterial* material, Model& model)
{
	float shininess;
	if ((material->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS) || shininess < 1.0f)
	{
		shininess = 1.0f;
	}
	float specular_scalar;
	if (material->Get(AI_MATKEY_SPECULAR_FACTOR, specular_scalar) != AI_SUCCESS)
	{
		specular_scalar = 0.01f;
	}

	std::vector<Texture*> diffuse;
	diffuse = loadMaterialTexture(material, aiTextureType::aiTextureType_DIFFUSE, 1);
	if (diffuse.size() == 0)
	{
		diffuse = loadMaterialTexture(material, aiTextureType::aiTextureType_BASE_COLOR, 1);
	}

	// potential race cond
	RenderSystem&		  rs = Engine::getInstance().getRenderSystem();
	ShaderProgramManager& spm = rs.getShaderProgramManager();
	if (diffuse.size() == 0)
	{
		// MaterialColor
		xm::vec3  color;
		aiColor4D _aicolor;

		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, _aicolor) != AI_SUCCESS)
		{
			color = xm::vec3(0.5f, 0.5f, 0.5f);
		}
		color = xm::vec3(_aicolor.r, _aicolor.g, _aicolor.b);

		ModelEntry<MaterialColor> entry;
		entry.m_mesh = mesh;
		entry.m_material.m_color.m_vector = color;
		entry.m_material.m_specular_scalar.m_scalar = specular_scalar;

		entry.m_material.m_shininess.m_scalar = shininess;
		entry.m_material.m_shader_program = spm.getShaderProgram(ShaderProgramType::DEFERED_COLOR);
		model.syncPushMeshColor(entry);
		return;
	}
	std::vector<Texture*> specular = loadMaterialTexture(material, aiTextureType::aiTextureType_SPECULAR, 1);
	if (specular.size() == 0)
	{
		std::vector<Texture*> normal;

		if (_normalmap)
		{
			normal = loadMaterialTexture(material, aiTextureType::aiTextureType_HEIGHT, 1); // TODO - TO NORMALS
		}

		if (normal.size() == 0)
		{
			// MaterialD
			ModelEntry<MaterialD> entry;
			entry.m_mesh = mesh;
			entry.m_material.m_diffuse.m_texture = diffuse[0];
			entry.m_material.m_specular_scalar.m_scalar = specular_scalar;

			entry.m_material.m_shininess.m_scalar = shininess;
			entry.m_material.m_shader_program = spm.getShaderProgram(ShaderProgramType::DEFERED_DIFFUSE);
			model.syncPushMeshD(entry);
			return;
		}
		std::vector<Texture*> height; // loadMaterialTexture(material, aiTextureType::aiTextureType_HEIGHT, 1); TODO - REAL HEIGHT
		if (height.size() == 0)
		{
			// MaterialDN
			ModelEntry<MaterialDN> entry;
			entry.m_mesh = mesh;
			entry.m_material.m_diffuse.m_texture = diffuse[0];
			entry.m_material.m_specular_scalar.m_scalar = specular_scalar;
			entry.m_material.m_normal.m_texture = normal[0];

			entry.m_material.m_shininess.m_scalar = shininess;
			entry.m_material.m_shader_program = spm.getShaderProgram(ShaderProgramType::DEFERED_DIFFUSE_NORMAL);
			model.syncPushMeshDN(entry);
			return;
		}
		// MaterialDNH
		ModelEntry<MaterialDNH> entry;
		entry.m_mesh = mesh;
		entry.m_material.m_diffuse.m_texture = diffuse[0];
		entry.m_material.m_specular_scalar.m_scalar = specular_scalar;
		entry.m_material.m_normal.m_texture = normal[0];
		entry.m_material.m_height.m_texture = height[0];

		entry.m_material.m_shininess.m_scalar = shininess;
		entry.m_material.m_shader_program = spm.getShaderProgram(ShaderProgramType::DEFERED_DIFFUSE_NORMAL_HEIGHT);
		model.syncPushMeshDNH(entry);
		return;
	}
	else
	{
		std::vector<Texture*> normal;

		if (_normalmap)
		{
			normal = loadMaterialTexture(material, aiTextureType::aiTextureType_HEIGHT, 1); // TODO - TO NORMALS
		}

		if (normal.size() == 0)
		{
			// MaterialDS

			ModelEntry<MaterialDS> entry;
			entry.m_mesh = mesh;
			entry.m_material.m_diffuse.m_texture = diffuse[0];
			entry.m_material.m_specular.m_texture = specular[0];

			entry.m_material.m_shininess.m_scalar = shininess;
			entry.m_material.m_shader_program = spm.getShaderProgram(ShaderProgramType::DEFERED_DIFFUSE_SPECULAR);
			model.syncPushMeshDS(entry);
			return;
		}
		std::vector<Texture*> height; // loadMaterialTexture(material, aiTextureType::aiTextureType_HEIGHT, 1); TODO
		if (height.size() == 0)
		{
			// MaterialDSN

			ModelEntry<MaterialDSN> entry;
			entry.m_mesh = mesh;
			entry.m_material.m_diffuse.m_texture = diffuse[0];
			entry.m_material.m_specular.m_texture = specular[0];
			entry.m_material.m_normal.m_texture = normal[0];

			entry.m_material.m_shininess.m_scalar = shininess;
			entry.m_material.m_shader_program = spm.getShaderProgram(ShaderProgramType::DEFERED_DIFFUSE_SPECULAR_NORMAL);
			model.syncPushMeshDSN(entry);
			return;
		}
		// MaterialDSNH
		ModelEntry<MaterialDSNH> entry;
		entry.m_mesh = mesh;
		entry.m_material.m_diffuse.m_texture = diffuse[0];
		entry.m_material.m_specular.m_texture = specular[0];
		entry.m_material.m_normal.m_texture = normal[0];
		entry.m_material.m_height.m_texture = height[0];

		entry.m_material.m_shininess.m_scalar = shininess;
		entry.m_material.m_shader_program = spm.getShaderProgram(ShaderProgramType::DEFERED_DIFFUSE_SPECULAR_NORMAL_HEIGHT);
		model.syncPushMeshDSNH(entry);
	}
}

std::vector<Texture*> ResourceManager::loadMaterialTexture(aiMaterial* material, aiTextureType type, uint max_count)
{
	uint count = material->GetTextureCount(type);

	if (count == 0)
	{
		return std::vector<Texture*>();
	}
	if (count > max_count)
	{
		LOG_WARNING_F("Textures of type [%d] > than max count [%d]", type, max_count);
		count = max_count;
	}

	std::vector<Texture*> res;
	res.reserve(count);

	for (uint i = 0; i < count; ++i)
	{
		aiString path;
		if (material->GetTexture(type, i, &path) != AI_SUCCESS)
		{
			LOG_WARNING_F("material->GetTexture returned != AI_SUCCESS while getting texture type [%d] ", type);
			continue;
		}

		fs::path file = m_textures_path;
		fs::path _dirty_path = path.C_Str();
		file.append(_dirty_path.filename().generic_string());

		std::string_view			hashed(path.C_Str());
		std::hash<std::string_view> hasher;

		size_t hash = hasher(hashed);

		Texture* cached = syncGetCachedModelTexture(hash);
		if (cached)
		{
			res.push_back(cached);
			continue;
		}

		Texture tex;
		if (!initLoadTexture(file.generic_string(), tex, true))
		{
			continue;
		}
		Texture* new_tex = syncPushModelTexture(tex);
		syncCacheModelTexture(hash, new_tex);

		res.push_back(new_tex);
	}
	return res;
}

Mesh* ResourceManager::syncEmplaceModelMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	std::lock_guard lk(m_meshes_mtx);
	return &m_model_meshes.emplace_back(std::move(vertices), std::move(indices));
}

Mesh* ResourceManager::syncEmplaceModelMesh(std::vector<VertexTB>& vertices, std::vector<uint>& indices)
{
	std::lock_guard lk(m_meshes_mtx);
	return &m_model_meshes.emplace_back(std::move(vertices), std::move(indices));
}

Texture* ResourceManager::syncGetCachedModelTexture(size_t hash)
{
	std::lock_guard lk(m_cache_mat_textures_mtx);
	auto			it = m_cache_model_textures.find(hash);
	if (it != m_cache_model_textures.end())
	{
		return it->second;
	}
	return nullptr;
}

Texture* ResourceManager::syncPushModelTexture(Texture& texture)
{
	std::lock_guard lk(m_texures_mtx);
	m_model_textures.push_back(std::move(texture));
	return &m_model_textures.back();
}

void ResourceManager::syncCacheModelTexture(size_t hash, Texture* texture)
{
	std::lock_guard lk(m_cache_mat_textures_mtx);
	m_cache_model_textures[hash] = texture;
}
