#include "Material.h"
#include "Texture.h"

void MaterialColor::applyImpl()
{
	m_color.apply(m_shader_program);
	m_specular.apply(m_shader_program);
}

void MaterialDiff::applyImpl()
{
	m_diffuse.apply(m_shader_program);
	m_diff_blends.apply(m_shader_program);
	m_specular_scalar.apply(m_shader_program);
}

void MaterialDiffSpec::applyImpl()
{
	m_diff_spec.apply(m_shader_program);
	m_spec_blends.apply(m_shader_program);
	m_diff_blends.apply(m_shader_program);
}

void MaterialDiffSpecNorm::applyImpl()
{
	m_diff_spec_norm.apply(m_shader_program);
	m_spec_blends.apply(m_shader_program);
	m_diff_blends.apply(m_shader_program);
}

void MaterialDiffSpecNormHeight::applyImpl()
{
	m_diff_spec_norm_height.apply(m_shader_program);
	m_spec_blends.apply(m_shader_program);
	m_diff_blends.apply(m_shader_program);
}
