#pragma once

#include "Defines.h"
#include "ShaderProgram.h"
#include "MatParams.h"

class Texture;

template <typename Derived>
struct Material
{
	void apply();

	MP_FloatScalar m_shininess{ "material.shininess" };
	ShaderProgram* m_shader_program = nullptr;
};


struct MaterialColor : public Material<MaterialColor>
{
	void applyImpl();

	MP_FloatVector m_color{ "material.color" };
	MP_FloatScalar m_specular_scalar{ "material.specular_scalar" };
};

struct MaterialD : public Material<MaterialD>
{
	void applyImpl();

	MP_Texture<0>  m_diffuse{ "material.diffuse" };
	MP_FloatScalar m_specular_scalar{ "material.specular_scalar" };
};

struct MaterialDS : public Material<MaterialDS>
{
	void applyImpl();

	MP_Texture<0>  m_diffuse{ "material.diffuse" };
	MP_Texture<1>  m_specular{ "material.specular" };
};

struct MaterialDN : public Material<MaterialDN>
{
	void applyImpl();

	MP_Texture<0> m_diffuse{ "material.diffuse" };
	MP_Texture<1> m_normal{ "material.normal" };
	MP_FloatScalar m_specular_scalar{ "material.specular_scalar" };
};

struct MaterialDNH : public Material<MaterialDNH>
{
	void applyImpl();

	MP_Texture<0>  m_diffuse{ "material.diffuse" };
	MP_Texture<1>  m_normal{ "material.normal" };
	MP_Texture<2>  m_height{ "material.height" };
	MP_FloatScalar m_specular_scalar{ "material.specular_scalar" };
};

// TODO: DH

struct MaterialDSN : public Material<MaterialDSN>
{
	void applyImpl();

	MP_Texture<0> m_diffuse{ "material.diffuse" };
	MP_Texture<1> m_specular{ "material.specular" };
	MP_Texture<2> m_normal{ "material.normal" };
};

struct MaterialDSNH : public Material<MaterialDSNH>
{
	void applyImpl();

	MP_Texture<0> m_diffuse{ "material.diffuse" };
	MP_Texture<1> m_specular{ "material.specular" };
	MP_Texture<2> m_normal{ "material.normal" };
	MP_Texture<3> m_height{ "material.height" };
};

template <typename Derived>
inline void Material<Derived>::apply()
{
	if (!m_shader_program)
	{
		LOG_ERROR_S("m_shader_program was invalid");
		return;
	}
	m_shininess.apply(m_shader_program);
	static_cast<Derived*>(this)->applyImpl();
}

