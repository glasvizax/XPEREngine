#include "Material.h"
#include "Texture.h"

void MaterialColor::applyImpl()
{
	m_color.apply(m_shader_program);
	m_specular_scalar.apply(m_shader_program);
}

void MaterialD::applyImpl()
{
	m_diffuse.apply(m_shader_program);
	m_specular_scalar.apply(m_shader_program);
}

void MaterialDS::applyImpl()
{
	m_diffuse.apply(m_shader_program);
	m_specular.apply(m_shader_program);
}

void MaterialDN::applyImpl()
{
	m_diffuse.apply(m_shader_program);
	m_normal.apply(m_shader_program);
	m_specular_scalar.apply(m_shader_program);
}

void MaterialDSN::applyImpl()
{
	m_diffuse.apply(m_shader_program);
	m_normal.apply(m_shader_program);
	m_specular.apply(m_shader_program);
}

void MaterialDSNH::applyImpl()
{
	m_diffuse.apply(m_shader_program);
	m_normal.apply(m_shader_program);
	m_specular.apply(m_shader_program);
	m_height.apply(m_shader_program);
}

void MaterialDNH::applyImpl()
{
	m_diffuse.apply(m_shader_program);
	m_normal.apply(m_shader_program);
	m_specular_scalar.apply(m_shader_program);
	m_height.apply(m_shader_program);
}
