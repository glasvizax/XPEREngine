#include "ShaderProgramManager.h"

bool ShaderProgramManager::init()
{
	Engine&			 engine = Engine::getInstance();
	ResourceManager& rm = engine.getResourceManager();

	ShaderProgram sp_color;
	if (!rm.initLoadShaderProgram("color_shader.vert", "color_shader.frag", sp_color))
	{
		return false;
	}
	
	m_default_shaders[ShaderProgramType::COLOR] = std::move(sp_color);

	ShaderProgram sp_diffuse;
	if (!rm.initLoadShaderProgram("diffuse_shader.vert", "diffuse_shader.frag", sp_diffuse))
	{
		return false;
	}
	
	m_default_shaders[ShaderProgramType::DIFFUSE] = std::move(sp_diffuse);
	LOG_INFO_S("test");

	return true;
}

ShaderProgram* ShaderProgramManager::getShaderProgramPtr(ShaderProgramType type)
{
	auto found = m_default_shaders.find(type);
	if (found == m_default_shaders.end())
	{
		LOG_ERROR_F("shader program of [%d] not found", type);
		return nullptr;
	}

	return &found->second;
}

std::string ShaderProgramManager::shaderProgramTypeName(ShaderProgramType type)
{
	switch (type)
	{
		case ShaderProgramType::COLOR:
			return "ShaderProgramType::COLOR";
		case ShaderProgramType::DIFFUSE:
			return "ShaderProgramType::DIFFUSE";
		case ShaderProgramType::DIFFUSE_SPECULAR:
			return "ShaderProgramType::DIFFUSE_SPECULAR";
		case ShaderProgramType::DIFFUSE_SPECULAR_NORMAL:
			return "ShaderProgramType::DIFFUSE_SPECULAR_NORMAL";
		case ShaderProgramType::DIFFUSE_SPECULAR_NORMAL_HEIGHT:
			return "ShaderProgramType::DIFFUSE_SPECULAR_NORMAL_HEIGHT";
	}
	return "ShaderProgramType::COLOR";
}