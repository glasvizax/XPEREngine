#include "ShaderProgramManager.h"

// clang-format off
std::unordered_map<ShaderProgramType, VertexFragmentNames> DEFAULT_SHADER_NAMES = {
	{ ShaderProgramType::COLOR,								{ "shader_color.vert",			"shader_color.frag" } },
	{ ShaderProgramType::DIFFUSE,							{ "shader_d.vert",				"shader_d.frag" } },
	{ ShaderProgramType::DIFFUSE_NORMAL,					{ "shader_dn.vert",				"shader_dn.frag" } },
	{ ShaderProgramType::DIFFUSE_NORMAL_HEIGHT,				{ "shader_dnh.vert",			"shader_dnh.frag" } },
	{ ShaderProgramType::DIFFUSE_SPECULAR,					{ "shader_ds.vert",				"shader_ds.frag" } },
	{ ShaderProgramType::DIFFUSE_SPECULAR_NORMAL,			{ "shader_dsn.vert",			"shader_dsn.frag" } },
	{ ShaderProgramType::DIFFUSE_SPECULAR_NORMAL_HEIGHT,	{ "shader_dsnh.vert",			"shader_dsnh.frag" } },
	{ ShaderProgramType::POSTPROCESS,						{ "shader_postprocess.vert",	"shader_postprocess.frag" } },
};

// clang-forat on

void ShaderProgramManager::init()
{
	Engine&			 engine = Engine::getInstance();
	ResourceManager& rm = engine.getResourceManager();

	loadShaders(ShaderProgramType::COLOR, rm);
	loadShaders(ShaderProgramType::DIFFUSE, rm);
	loadShaders(ShaderProgramType::DIFFUSE_SPECULAR, rm);
	loadShaders(ShaderProgramType::DIFFUSE_SPECULAR_NORMAL, rm);
	loadShaders(ShaderProgramType::DIFFUSE_SPECULAR_NORMAL_HEIGHT, rm);
	loadShaders(ShaderProgramType::POSTPROCESS, rm);
}

// should be thread safe ...
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
		case ShaderProgramType::POSTPROCESS:
			return "ShaderProgramType::POSTPROCESS";
	}
	return "ShaderProgramType::COLOR";
}

void ShaderProgramManager::loadShaders(ShaderProgramType type, ResourceManager& resource_manager)
{
	auto names = DEFAULT_SHADER_NAMES[type];
	
	ShaderProgram shader_program;
	if (!resource_manager.initLoadShaderProgram(names.vertex, names.fragment, shader_program))
	{
		LOG_ERROR_F("couldn't load shader : [%s]", shaderProgramTypeName(type).c_str());
		return;
	}

	m_default_shaders[type] = std::move(shader_program);
}
