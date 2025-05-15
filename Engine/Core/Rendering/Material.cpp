#include "Material.h"
#include "Texture.h"

void MaterialC::applyImpl()
{
	m_color.apply(m_shader_program);
	m_specular.apply(m_shader_program);
}

void MaterialD::applyImpl()
{
	m_diffuse.apply(m_shader_program);
	m_diff_blends.apply(m_shader_program);
	m_specular_scalar.apply(m_shader_program);
}

void MaterialDS::applyImpl()
{
	m_diff_spec.apply(m_shader_program);
	m_spec_blends.apply(m_shader_program);
	m_diff_blends.apply(m_shader_program);
}

void MaterialDSNH::applyImpl()
{
	m_diff_spec_norm_height.apply(m_shader_program);
	m_spec_blends.apply(m_shader_program);
	m_diff_blends.apply(m_shader_program);
}

void MaterialDSN::applyImpl()
{
	m_diff_spec_norm.apply(m_shader_program);
	m_spec_blends.apply(m_shader_program);
	m_diff_blends.apply(m_shader_program);
}
