#include "Material.h"
#include "Texture.h"

void MaterialC::apply_impl()
{
	m_shader_program->setVec("material.color", m_color);
	m_shader_program->set("material.specular", m_specular);
}

void MaterialD::apply_impl()
{
	size_t size = m_diffuses.size();

	if (m_diffuses.size() > 16)
	{
		LOG_WARNING_S("in DiffuseMaterial m_diffuses.size() should be <= 16");
		size = 16;
	}

	if (m_blend_coeffs.size() < size)
	{
		LOG_WARNING_S("in DiffuseMaterial m_blend_coefficients.size() should be <= 16 and <= m_diffuses.size()");
		size = m_blend_coeffs.size();
	}

	for (int i = 0; i < size; ++i)
	{
		m_diffuses[i]->bind(i);
	}
	m_shader_program->setArray("material.diffuses", units, size);
	m_shader_program->set("material.diffuses_num", size);
	m_shader_program->setArray("material.blend_coeffs", m_blend_coeffs.data(), size);
	m_shader_program->set("material.specular", m_specular);
}

void MaterialDS::apply_impl()
{
	size_t diffs_size = m_diffuses.size();
	if (diffs_size > 8)
	{
		LOG_WARNING_S("in MaterialDS m_diffuses.size() should be <= 8");
		diffs_size = 8;
	}

	if (m_diff_blend_coeffs.size() < diffs_size)
	{
		LOG_WARNING_S("in MaterialDS m_diff_blend_coeffs.size() should be <= 8 and <= m_diffuses.size()");
		diffs_size = m_diff_blend_coeffs.size();
	}

	size_t specs_size = m_speculars.size();
	if (specs_size > 8)
	{
		LOG_WARNING_S("in MaterialDS m_diffuses.size() should be <= 8");
		specs_size = 8;
	}

	if (m_spec_blend_coeffs.size() < specs_size)
	{
		LOG_WARNING_S("in MaterialDS m_spec_blend_coeffs.size() should be <= 8 and <= m_speculars.size()");
		specs_size = m_spec_blend_coeffs.size();
	}

	for (int i = 0; i < diffs_size; ++i)
	{
		m_diffuses[i]->bind(i);
	}

	for (int i = 0; i < specs_size; ++i)
	{
		m_speculars[i]->bind(8 + i); // Юниты 8-15
	}

	m_shader_program->setArray("material.diffuses", units, diffs_size);
	m_shader_program->setArray("material.speculars", units + 8, specs_size);
	m_shader_program->set("material.diffuses_num", diffs_size);
	m_shader_program->set("material.speculars_num", specs_size);
	m_shader_program->setArray("material.diff_blend_coeffs", m_diff_blend_coeffs.data(), diffs_size);
	m_shader_program->setArray("material.spec_blend_coeffs", m_spec_blend_coeffs.data(), specs_size);
}
