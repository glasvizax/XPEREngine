#include "ShaderProgramManager.h"

// clang-format off
std::unordered_map<ShaderProgramType, VertexFragmentNames> DEFAULT_SHADER_NAMES = {
	{ ShaderProgramType::GEOMETRY_COLOR,							{ "geometry_color_mat.vert",	"geometry_color_mat.frag" } },
	{ ShaderProgramType::GEOMETRY_DIFFUSE,							{ "geometry_d_mat.vert",		"geometry_d_mat.frag" } },
	{ ShaderProgramType::GEOMETRY_DIFFUSE_NORMAL,					{ "geometry_dn_mat.vert",		"geometry_dn_mat.frag" } },
	{ ShaderProgramType::GEOMETRY_DIFFUSE_NORMAL_HEIGHT,			{ "geometry_dnh_mat.vert",		"geometry_dnh_mat.frag" } },
	{ ShaderProgramType::GEOMETRY_DIFFUSE_SPECULAR,					{ "geometry_ds_mat.vert",		"geometry_ds_mat.frag" } },
	{ ShaderProgramType::GEOMETRY_DIFFUSE_SPECULAR_NORMAL,			{ "geometry_dsn_mat.vert",		"geometry_dsn_mat.frag" } },
	{ ShaderProgramType::GEOMETRY_DIFFUSE_SPECULAR_NORMAL_HEIGHT,	{ "geometry_dsnh_mat.vert",		"geometry_dsnh_mat.frag" } },
	
	{ ShaderProgramType::LIGHTING_AMBIENT,							{ "lighting_ambient.vert",		"lighting_ambient.frag" } },
	{ ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR,					{ "lighting_diffspec.vert",		"lighting_diffspec.frag" } },
	{ ShaderProgramType::LIGHTING_SSAO_BASE,						{ "lighting_ssao_base.vert",	"lighting_ssao_base.frag" } },
	{ ShaderProgramType::LIGHTING_SSAO_BLUR,						{ "lighting_ssao_blur.vert",	"lighting_ssao_blur.frag" } },

	{ ShaderProgramType::POSTPROCESS,								{ "postprocess.vert",			"postprocess.frag" } }
};

// clang-forat on

void ShaderProgramManager::init()
{
	Engine&			 engine = Engine::getInstance();
	ResourceManager& rm = engine.getResourceManager();

	loadShaders(ShaderProgramType::GEOMETRY_COLOR, rm);
	loadShaders(ShaderProgramType::GEOMETRY_DIFFUSE, rm);
	loadShaders(ShaderProgramType::GEOMETRY_DIFFUSE_SPECULAR, rm);
	loadShaders(ShaderProgramType::GEOMETRY_DIFFUSE_SPECULAR_NORMAL, rm);
	loadShaders(ShaderProgramType::GEOMETRY_DIFFUSE_SPECULAR_NORMAL_HEIGHT, rm);
	loadShaders(ShaderProgramType::POSTPROCESS, rm);

	loadShaders(ShaderProgramType::LIGHTING_SSAO_BASE, rm);
	loadShaders(ShaderProgramType::LIGHTING_SSAO_BLUR, rm);
	loadShaders(ShaderProgramType::LIGHTING_AMBIENT, rm);
	loadShaders(ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR, rm);
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
		case ShaderProgramType::GEOMETRY_COLOR:
			return "ShaderProgramType::GEOMETRY_COLOR";
		case ShaderProgramType::GEOMETRY_DIFFUSE:
			return "ShaderProgramType::GEOMETRY_DIFFUSE";
		case ShaderProgramType::GEOMETRY_DIFFUSE_NORMAL:
			return "ShaderProgramType::GEOMETRY_DIFFUSE_NORMAL";
		case ShaderProgramType::GEOMETRY_DIFFUSE_NORMAL_HEIGHT:
			return "ShaderProgramType::GEOMETRY_DIFFUSE_NORMAL_HEIGHT";
		case ShaderProgramType::GEOMETRY_DIFFUSE_SPECULAR_NORMAL:
			return "ShaderProgramType::GEOMETRY_DIFFUSE_SPECULAR_NORMAL";

		case ShaderProgramType::LIGHTING_AMBIENT:
			return "ShaderProgramType::LIGHTING_AMBIENT";
		case ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR:
			return "ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR";
		case ShaderProgramType::LIGHTING_SSAO_BASE:
			return "ShaderProgramType::LIGHTING_SSAO_BASE";
		case ShaderProgramType::LIGHTING_SSAO_BLUR:
			return "ShaderProgramType::LIGHTING_SSAO_BLUR";

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
