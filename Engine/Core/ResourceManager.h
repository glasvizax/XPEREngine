#pragma once

#include <filesystem>
#include <string>

class ShaderProgram;

class ResourceManager
{
	friend class Engine;

	bool init();

	void destroy();

public:
	bool loadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, ShaderProgram& shader_program);
	bool loadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, const std::string& geometry_name, ShaderProgram& shader_program);

	bool readFile(const std::filesystem::path& path, std::string& content);

private:
	std::filesystem::path m_current_path;
};
