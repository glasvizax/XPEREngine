#include "ResourceManager.h"

#include <fstream>

#include <stb_image.h>

#include "ShaderProgram.h"
#include "Debug.h"
#include "Texture.h"

using namespace std::string_literals;

bool ResourceManager::init()
{
	m_current_path = std::filesystem::current_path();
	return true;
}

void ResourceManager::destroy()
{
}

bool ResourceManager::initLoadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, ShaderProgram& shader_program)
{
	std::string			  vertex_src, fragment_src;
	std::filesystem::path current = m_current_path;
	if (!readFile(current.concat("\\").concat(vertex_name), vertex_src))
	{
		return false;
	}

	current = m_current_path;
	if (!readFile(current.concat("\\").concat(fragment_name), fragment_src))
	{
		return false;
	}

	return shader_program.init(vertex_src.c_str(), fragment_src.c_str());
}

bool ResourceManager::initLoadShaderProgram(const std::string& vertex_name, const std::string& fragment_name, const std::string& geometry_name, ShaderProgram& shader_program)
{
	std::string vertex_src, fragment_src, geometry_src;

	std::filesystem::path current = m_current_path;
	if (!readFile(current.concat("\\").concat(vertex_name), vertex_src))
	{
		return false;
	}
	current = m_current_path;

	if (!readFile(current.concat("\\").concat(fragment_name), fragment_src))
	{
		return false;
	}

	current = m_current_path;
	if (!readFile(current.concat("\\").concat(fragment_name), fragment_src))
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
		LOG_ERROR_F("couldn't load file [%s]", name.c_str());
		return false;
	}

	GLint format = GL_RGB;
	GLint internal_format = GL_RGB8;
	switch (channels_num)
	{
		case (1):
			format = GL_RED;
			internal_format = GL_R8;
			break;
		case (2):
			format = GL_RG;
			internal_format = GL_RG8;
			break;
		case (4):
			format = GL_RGBA;
			internal_format = GL_RGBA8;
			break;
		default:
			break;
	}
	texture.init(width, height, internal_format, generate_mipmap);
	texture.loadData(GL_UNSIGNED_BYTE, format, data);

	stbi_image_free(data);

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
