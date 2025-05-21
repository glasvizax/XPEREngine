#pragma once
#include <unordered_map>

#include "ShaderProgram.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "Debug.h"

enum class ShaderProgramType : uint8_t
{
	DEFERED_COLOR,
	DEFERED_DIFFUSE,
	DEFERED_DIFFUSE_NORMAL,
	DEFERED_DIFFUSE_NORMAL_HEIGHT,
	DEFERED_DIFFUSE_SPECULAR,
	DEFERED_DIFFUSE_SPECULAR_NORMAL,
	DEFERED_DIFFUSE_SPECULAR_NORMAL_HEIGHT,

	FORWARD_COLOR,

	LIGHTING_SSAO_BASE,
	LIGHTING_SSAO_BLUR,
	LIGHTING_DEPTHMAP,
	LIGHTING_AMBIENT,
	LIGHTING_DIFFUSE_SPECULAR,

	POSTPROCESS
};

struct ShadersNames
{
	std::string vertex;
	std::string fragment;
	std::string geometry;
};

class ShaderProgramManager
{
	friend class RenderSystem;
	void		   init();

public:
	
	ShaderProgramManager() = default;
	~ShaderProgramManager() = default;
	ShaderProgramManager(const ShaderProgramManager&) = delete;
	ShaderProgramManager& operator=(const ShaderProgramManager&) = delete;
	ShaderProgramManager(ShaderProgramManager&&) noexcept = delete;
	ShaderProgramManager& operator=(ShaderProgramManager&&) noexcept = delete;
	
	ShaderProgram* getShaderProgram(ShaderProgramType type);

	std::string ShaderProgramManager::shaderProgramTypeName(ShaderProgramType type);

private:
	std::unordered_map<ShaderProgramType, ShaderProgram> m_default_shaders;
	
	void loadShaders(ShaderProgramType type, ResourceManager& resource_manager);
};
