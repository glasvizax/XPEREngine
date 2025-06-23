#pragma once

#include "xm/xm.h"

struct PointLight
{
	xm::vec3 m_position;
	float	 m_linear;
	xm::vec3 m_ambient;
	float	 m_quadratic;
	xm::vec3 m_diffuse;
	float	 m_radius; // actually pad
	xm::vec3 m_specular;
	float	 _PAD_ = 0.0f;
};

struct DirectionalLight
{
	xm::vec3 m_diffuse;
	xm::vec3 m_specular;
	float	 _PAD_ = 0.0f;
};

using DirLight = DirectionalLight;