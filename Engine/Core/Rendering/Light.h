#pragma once

#include <glm/glm.hpp>

struct PointLight
{
	glm::vec3 m_position;
	float	  m_linear;
	glm::vec3 m_ambient;
	float	  m_quadratic;
	glm::vec3 m_diffuse;
	float	  m_radius; // actually pad
	glm::vec3 m_specular;
	float	  _PAD_ = 0.0f;
};

struct DirectionalLight
{
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float	  _PAD_ = 0.0f;
};

using DirLight = DirectionalLight;