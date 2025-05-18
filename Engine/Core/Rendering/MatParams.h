#pragma once

#include <unordered_map>
#include <limits>
#include <array>

#include "Defines.h"
#include "ShaderProgram.h"
#include "Texture.h"

struct MP_FloatScalar
{
	MP_FloatScalar(const std::string& name)
		: m_param_name(name) {}
	float m_scalar;

	void apply(ShaderProgram* program)
	{
		program->set(m_param_name, m_scalar);
	}

	std::string m_param_name = "material.diffuse";
};

struct MP_FloatVector
{
	MP_FloatVector(const std::string& name)
		: m_param_name(name) {}
	
	glm::vec3 m_vector;

	void apply(ShaderProgram* program)
	{
		program->setVec(m_param_name, m_vector);
	}

	std::string m_param_name = "material.diffuse";
};

template <int I>
struct MP_Texture
{
	MP_Texture(const std::string& name)
		: m_param_name(name) {}
	
	void apply(ShaderProgram* program)
	{
		if (m_first)
		{
			program->set(m_param_name, I);
		}
		if (m_texture)
		{
			m_texture->bind(I);
		}
	}

	std::string m_param_name = "material.diffuse";
	Texture* m_texture;

private:
	bool	 m_first = true;
};

