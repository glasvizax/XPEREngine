#include "ResourceManager.h"

#include <fstream>

#include "ShaderProgram.h"
#include "Debug.h"

using namespace std::string_literals;

bool ResourceManager::init()
{
	m_current_path = std::filesystem::current_path();
	return true;
}

void ResourceManager::destroy()
{

}

ShaderProgram ResourceManager::loadShaderProgram(const std::string& vertex_name, const std::string& fragment_name)
{

	std::filesystem::path current = m_current_path;
	std::string vertex_src;
	readFile(current.concat("\\").concat(vertex_name), vertex_src);

	current = m_current_path;
	std::string fragment_src;
	readFile(current.concat("\\").concat(fragment_name), fragment_src);

	ShaderProgram program(vertex_src.c_str(), fragment_src.c_str());

	return program;
}

ShaderProgram ResourceManager::loadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, const std::string& geometry_name)
{
	std::string vertex_src;

	std::filesystem::path current = m_current_path;
	readFile(current.concat("\\").concat(vertex_name), vertex_src);

	current = m_current_path;
	std::string fragment_src;
	readFile(current.concat("\\").concat(fragment_name), fragment_src);
	
	current = m_current_path;
	std::string geometry_src;
	readFile(current.concat("\\").concat(fragment_name), fragment_src);

	ShaderProgram program(vertex_src.c_str(), fragment_src.c_str(), geometry_src.c_str());

	return program;
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

