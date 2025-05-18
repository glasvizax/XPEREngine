#pragma once

#include <deque>
#include <filesystem>
#include <string>
#include <mutex>

#include <assimp/material.h>

#include "Texture.h"
#include "Mesh.h"

struct aiMesh;
struct aiNode;
struct aiScene;

class ShaderProgram;
class Texture;
class Entity;
struct Model;

namespace fs = std::filesystem;

class ResourceManager
{
	friend class Engine;

	bool init();
	void destroy();

public:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) noexcept = delete;
	ResourceManager& operator=(ResourceManager&&) noexcept = delete;

	bool initLoadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, ShaderProgram& shader_program);
	bool initLoadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, const std::string& geometry_name, ShaderProgram& shader_program);

	bool initLoadTexture(const std::string& path, Texture& texture, bool generate_mipmap);

	bool loadModel(const std::string path, Entity& root_entity, bool flip_uv = true);

	bool readFile(const std::filesystem::path& path, std::string& content);

	std::vector<fs::path> findFiles(const std::string& filename);

private:
	void processNode(aiNode* node, const aiScene* scene, Entity* parent);

	void processMesh(aiMesh* mesh, const aiScene* scene, Model& model);

	void processMaterial(Mesh* mesh, aiMaterial* material, Model& model);

	std::vector<Texture*> loadMaterialTexture(aiMaterial* material, aiTextureType type, uint max_count);

	fs::path m_current_path;
	fs::path m_textures_path;

	Mesh* syncEmplaceMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices);

	Texture* syncGetCachedTexture(size_t hash);

	Texture* syncPushTexture(Texture& texture);

	void syncCacheTexture(size_t hash, Texture* texture);

	std::deque<Mesh>					 m_meshes;
	std::deque<Texture>					 m_textures;
	std::unordered_map<size_t, Texture*> m_cache_mat_textures;

	std::mutex m_meshes_mtx;
	std::mutex m_texures_mtx;
	std::mutex m_cache_mat_textures_mtx;
};
