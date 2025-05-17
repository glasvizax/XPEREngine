#pragma once

#include "Defines.h"
#include "ShaderProgram.h"
#include "MatParams.h"

class Texture;

template <typename Derived>
struct Material
{
	void apply();

	MP_Shininess m_shininess;

	ShaderProgram* m_shader_program = nullptr;
};

/// <summary>
/// color
/// </summary>
struct MaterialColor : public Material<MaterialColor>
{
	void applyImpl();

	MP_Color		  m_color;
	MP_SpecularScalar m_specular;
};

/// <summary>
/// 16 diffuse
/// </summary>
struct MaterialDiff : public Material<MaterialDiff>
{
	void applyImpl();

	MP_Diffuse16	   m_diffuse;
	MP_DiffuseBlends16 m_diff_blends;
	MP_SpecularScalar  m_specular_scalar;
};

/// <summary>
/// 8 diffuse + 8 specular
/// </summary>
struct MaterialDiffSpec : public Material<MaterialDiffSpec>
{
	void applyImpl();

	MP_Diff8Spec8	   m_diff_spec;
	MP_SpecularBlends8 m_spec_blends;
	MP_DiffuseBlends8  m_diff_blends;
};

/// <summary>
// 7 diffuse + 7 specular + normal
/// </summary>
struct MaterialDiffSpecNorm : public Material<MaterialDiffSpecNorm>
{
	void applyImpl();

	MP_Diff7Spec7Norm  m_diff_spec_norm;
	MP_SpecularBlends8 m_spec_blends;
	MP_DiffuseBlends8  m_diff_blends;
};

/// <summary>
/// 7 diffuse + 7 specular + normal + height
/// </summary>
struct MaterialDiffSpecNormHeight : public Material<MaterialDiffSpecNormHeight>
{
	void applyImpl();

	MP_Diff7Spec7NormHeight m_diff_spec_norm_height;
	MP_SpecularBlends8		m_spec_blends;
	MP_DiffuseBlends8		m_diff_blends;
};


template <typename Derived>
inline void Material<Derived>::apply()
{
	if (!m_shader_program)
	{
		LOG_ERROR_F("ShaderProgram invalid in %s", typeid(Derived).name());
		return;
	}
	m_shader_program->use();
	m_shininess.apply(m_shader_program);
	static_cast<Derived*>(this)->applyImpl();
}
