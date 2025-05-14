#pragma once

#include "Defines.h"
#include "ShaderProgram.h"

class Texture;

template <typename Derived>
struct Material
{
	Material()
		: m_id(s_count++)
	{
	}

	void apply();

	float m_shininess = 32.0f;

	ShaderProgram* m_shader_program = nullptr;

protected:
	inline static uint s_applied_id = -1;
	inline static uint s_count = 0;
	inline static uint units[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	uint m_id;
};

// color
struct MaterialC : public Material<MaterialC>
{
	using Material::Material;

	void apply_impl();

	glm::vec4 m_color = glm::vec4(glm::vec3(0.1f), 1.0f);
	float	  m_specular = 0.1f;
};

// diffuses
struct MaterialD : public Material<MaterialD>
{
	using Material::Material;

	void apply_impl();

	std::vector<Texture*> m_diffuses;
	std::vector<float>	  m_blend_coeffs = std::vector<float>(1, 1.0f);
	float				  m_specular = 0.1f;
};

// diffuses + speculars
struct MaterialDS : public Material<MaterialDS>
{
	using Material::Material;

	void apply_impl();

	std::vector<Texture*> m_diffuses;
	std::vector<Texture*> m_speculars;
	std::vector<float>	  m_diff_blend_coeffs = std::vector<float>(1, 1.0f);
	std::vector<float>	  m_spec_blend_coeffs = std::vector<float>(1, 1.0f);
};

//TODO DSN + DSNH

template <typename Derived>
inline void Material<Derived>::apply()
{
	if (s_applied_id == m_id)
	{
		return;
	}
	s_applied_id = m_id;
	if (!m_shader_program)
	{
		LOG_ERROR_F("ShaderProgram invalid in %s", typeid(Derived).name());
		return;
	}
	m_shader_program->use();
	m_shader_program->set("material.shininess", m_shininess);
	static_cast<Derived*>(this)->apply_impl();
}
