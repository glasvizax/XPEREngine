#pragma once
#include <unordered_map>

#include "ShaderProgram.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "Debug.h"

enum class ShaderProgramType : uint8_t
{
	COLOR,
	DIFFUSE,
	DIFFUSE_SPECULAR,
	DIFFUSE_SPECULAR_NORMAL,
	DIFFUSE_SPECULAR_NORMAL_HEIGHT
};

class ShaderProgramManager
{
	friend class RenderSystem;
	bool		   init();

public:
	ShaderProgram* getShaderProgramPtr(ShaderProgramType type);

	std::string ShaderProgramManager::shaderProgramTypeName(ShaderProgramType type);

private:
	std::unordered_map<ShaderProgramType, ShaderProgram> m_default_shaders;
};
