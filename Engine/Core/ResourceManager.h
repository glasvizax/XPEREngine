#pragma once

#include <filesystem>
#include <string>

class ShaderProgram;
class Texture;

class ResourceManager
{
	friend class Engine;

	bool init();

	void destroy();

public:
	bool initLoadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, ShaderProgram& shader_program);
	bool initLoadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, const std::string& geometry_name, ShaderProgram& shader_program);

	bool initLoadTexture(const std::string& name, Texture& texture, bool generate_mipmap);

	bool initLoadModel(const std::string& path);

	bool readFile(const std::filesystem::path& path, std::string& content);

private:
	std::filesystem::path m_current_path;
};
