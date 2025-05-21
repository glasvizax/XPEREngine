#include "ShaderProgramManager.h"

// clang-format off
std::unordered_map<ShaderProgramType, ShadersNames> DEFAULT_SHADER_NAMES = {
	{ ShaderProgramType::DEFERED_COLOR,								{ "defered_color_mat.vert",		"defered_color_mat.frag" } },
	{ ShaderProgramType::DEFERED_DIFFUSE,							{ "defered_d_mat.vert",			"defered_d_mat.frag" } },
	{ ShaderProgramType::DEFERED_DIFFUSE_NORMAL,					{ "defered_dn_mat.vert",		"defered_dn_mat.frag" } },
	{ ShaderProgramType::DEFERED_DIFFUSE_NORMAL_HEIGHT,				{ "defered_dnh_mat.vert",		"defered_dnh_mat.frag" } },
	{ ShaderProgramType::DEFERED_DIFFUSE_SPECULAR,					{ "defered_ds_mat.vert",		"defered_ds_mat.frag" } },
	{ ShaderProgramType::DEFERED_DIFFUSE_SPECULAR_NORMAL,			{ "defered_dsn_mat.vert",		"defered_dsn_mat.frag" } },
	{ ShaderProgramType::DEFERED_DIFFUSE_SPECULAR_NORMAL_HEIGHT,	{ "defered_dsnh_mat.vert",		"defered_dsnh_mat.frag" } },

	{ ShaderProgramType::FORWARD_COLOR,								{ "forward_color_mat.vert",		"forward_color_mat.frag" } },
	
	{ ShaderProgramType::LIGHTING_SSAO_BASE,						{ "lighting_ssao_base.vert",	"lighting_ssao_base.frag" } },
	{ ShaderProgramType::LIGHTING_SSAO_BLUR,						{ "lighting_ssao_blur.vert",	"lighting_ssao_blur.frag" } },
	{ ShaderProgramType::LIGHTING_AMBIENT,							{ "lighting_ambient.vert",		"lighting_ambient.frag" } },
	{ ShaderProgramType::LIGHTING_DEPTHMAP,							{ "depthmap.vert",				"depthmap.frag",				"depthmap.geom"	} },
	{ ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR,					{ "lighting_diffspec.vert",		"lighting_diffspec.frag" } },

	{ ShaderProgramType::POSTPROCESS,								{ "postprocess.vert",			"postprocess.frag" } }
};

// clang-forat on

void ShaderProgramManager::init()
{
	Engine&			 engine = Engine::getInstance();
	ResourceManager& rm = engine.getResourceManager();

	loadShaders(ShaderProgramType::DEFERED_COLOR, rm);
	loadShaders(ShaderProgramType::DEFERED_DIFFUSE, rm);
	loadShaders(ShaderProgramType::DEFERED_DIFFUSE_SPECULAR, rm);
	loadShaders(ShaderProgramType::DEFERED_DIFFUSE_SPECULAR_NORMAL, rm);
	loadShaders(ShaderProgramType::DEFERED_DIFFUSE_SPECULAR_NORMAL_HEIGHT, rm);

	loadShaders(ShaderProgramType::FORWARD_COLOR, rm);

	loadShaders(ShaderProgramType::LIGHTING_SSAO_BASE, rm);
	loadShaders(ShaderProgramType::LIGHTING_SSAO_BLUR, rm);
	loadShaders(ShaderProgramType::LIGHTING_AMBIENT, rm);

	loadShaders(ShaderProgramType::LIGHTING_DEPTHMAP, rm);
	loadShaders(ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR, rm);
	loadShaders(ShaderProgramType::POSTPROCESS, rm);
}

// should be thread safe ...
ShaderProgram* ShaderProgramManager::getShaderProgram(ShaderProgramType type)
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
		case ShaderProgramType::DEFERED_COLOR:
			return "ShaderProgramType::DEFERED_COLOR";
		case ShaderProgramType::DEFERED_DIFFUSE:
			return "ShaderProgramType::DEFERED_DIFFUSE";
		case ShaderProgramType::DEFERED_DIFFUSE_NORMAL:
			return "ShaderProgramType::DEFERED_DIFFUSE_NORMAL";
		case ShaderProgramType::DEFERED_DIFFUSE_NORMAL_HEIGHT:
			return "ShaderProgramType::DEFERED_DIFFUSE_NORMAL_HEIGHT";
		case ShaderProgramType::DEFERED_DIFFUSE_SPECULAR_NORMAL:
			return "ShaderProgramType::DEFERED_DIFFUSE_SPECULAR_NORMAL";

		case ShaderProgramType::FORWARD_COLOR:
			return "ShaderProgramType::FORWARD_COLOR";

		case ShaderProgramType::LIGHTING_AMBIENT:
			return "ShaderProgramType::LIGHTING_AMBIENT";
		case ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR:
			return "ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR";
		case ShaderProgramType::LIGHTING_DEPTHMAP:
			return "ShaderProgramType::LIGHTING_DEPTHMAP";

		case ShaderProgramType::LIGHTING_SSAO_BASE:
			return "ShaderProgramType::LIGHTING_SSAO_BASE";
		case ShaderProgramType::LIGHTING_SSAO_BLUR:
			return "ShaderProgramType::LIGHTING_SSAO_BLUR";

		case ShaderProgramType::POSTPROCESS:
			return "ShaderProgramType::POSTPROCESS";
	}
	return "-";
}

void ShaderProgramManager::loadShaders(ShaderProgramType type, ResourceManager& resource_manager)
{
	auto names = DEFAULT_SHADER_NAMES[type];
	
	ShaderProgram shader_program;

	if(names.geometry.empty())
	{
		if (!resource_manager.initLoadShaderProgram(names.vertex, names.fragment, shader_program))
		{
			LOG_ERROR_F("couldn't load shader : [%s]", shaderProgramTypeName(type).c_str());
			return;
		}	
	}
	else 
	{
		if (!resource_manager.initLoadShaderProgram(names.vertex, names.fragment, names.geometry, shader_program))
		{
			LOG_ERROR_F("couldn't load shader : [%s]", shaderProgramTypeName(type).c_str());
			return;
		}	
	}

	m_default_shaders[type] = std::move(shader_program);
}
