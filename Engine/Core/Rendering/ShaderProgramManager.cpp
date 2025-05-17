#include "ShaderProgramManager.h"

// clang-format off
std::unordered_map<ShaderProgramType, VertexFragmentNames> DEFAULT_SHADER_NAMES = {
	{ ShaderProgramType::COLOR,								{ "color_shader.vert",	"color_shader.frag" } },
	{ ShaderProgramType::DIFFUSE,							{ "diffuse_shader.vert", "diffuse_shader.frag" } },
	{ ShaderProgramType::DIFFUSE_SPECULAR,					{ "diffuse_specular_shader.vert", "diffuse_specular_shader.frag" } },
	{ ShaderProgramType::DIFFUSE_SPECULAR_NORMAL,			{ "diffuse_specular_normal_shader.vert", "diffuse_specular_normal_shader.frag" } },
	{ ShaderProgramType::DIFFUSE_SPECULAR_NORMAL_HEIGHT,	{ "diffuse_specular_normal_height_shader.vert", "diffuse_specular_normal_height_shader.frag" } },
};

// clang-format on
bool ShaderProgramManager::init()
{
	Engine&			 engine = Engine::getInstance();
	ResourceManager& rm = engine.getResourceManager();

	auto color_names = DEFAULT_SHADER_NAMES[ShaderProgramType::COLOR];
	auto diff_names = DEFAULT_SHADER_NAMES[ShaderProgramType::DIFFUSE];
	auto diff_spec_names = DEFAULT_SHADER_NAMES[ShaderProgramType::DIFFUSE_SPECULAR];
	auto diff_spec_norm_names = DEFAULT_SHADER_NAMES[ShaderProgramType::DIFFUSE_SPECULAR_NORMAL];
	auto diff_spec_norm_height_names = DEFAULT_SHADER_NAMES[ShaderProgramType::DIFFUSE_SPECULAR_NORMAL_HEIGHT];

	ShaderProgram sp_color;
	if (!rm.initLoadShaderProgram(color_names.vertex, color_names.fragment, sp_color))
	{
		return false;
	}

	m_default_shaders[ShaderProgramType::COLOR] = std::move(sp_color);

	ShaderProgram sp_diff;
	if (!rm.initLoadShaderProgram(diff_names.vertex, diff_names.fragment, sp_diff))
	{
		return false;
	}
	m_default_shaders[ShaderProgramType::DIFFUSE] = std::move(sp_diff);

	ShaderProgram sp_diff_spec;
	if (!rm.initLoadShaderProgram(diff_spec_names.vertex, diff_spec_names.fragment, sp_diff_spec))
	{
		return false;
	}
	m_default_shaders[ShaderProgramType::DIFFUSE_SPECULAR] = std::move(sp_diff_spec);

	ShaderProgram sp_diff_spec_norm;
	if (!rm.initLoadShaderProgram(diff_spec_norm_names.vertex, diff_spec_norm_names.fragment, sp_diff_spec_norm))
	{
		return false;
	}
	m_default_shaders[ShaderProgramType::DIFFUSE_SPECULAR_NORMAL] = std::move(sp_diff_spec_norm);

	ShaderProgram sp_diff_spec_norm_height;
	if (!rm.initLoadShaderProgram(diff_spec_norm_height_names.vertex, diff_spec_norm_height_names.fragment, sp_diff_spec_norm_height))
	{
		return false;
	}
	m_default_shaders[ShaderProgramType::DIFFUSE_SPECULAR_NORMAL_HEIGHT] = std::move(sp_diff_spec_norm_height);

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