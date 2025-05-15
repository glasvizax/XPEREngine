#include "MatParams.h"

inline static int units[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

void MP_Color::apply(ShaderProgram* program)
{
	for (int i = 0; i < 4; ++i)
	{	
		if (m_value[i] != s_global[i])
		{
			program->setVec(s_name, m_value);
			s_global = m_value;
			return;
		}
	}
}

void MP_SpecularScalar::apply(ShaderProgram* program)
{
	if (m_value != s_global)
	{
		program->set(s_name, m_value);
		s_global = m_value;
	}
}

void MP_Shininess::apply(ShaderProgram* program)
{
	if (m_value != s_global)
	{
		program->set(s_name, m_value);
		s_global = m_value;
	}
}

void MP_DiffuseBlends16::apply(ShaderProgram* program)
{
	for (int i = 0; i < 16; ++i)
	{
		if (m_value[i] != s_global[i])
		{
			program->setArray(s_name, &m_value[0], 16);

			for (int j = i; j < 16; ++j)
			{
				s_global[j] = m_value[j];
			}
			return;
		}
	}
}

void MP_DiffuseBlends8::apply(ShaderProgram* program)
{
	for (int i = 0; i < 8; ++i)
	{
		if (m_value[i] != s_global[i])
		{
			program->setArray(s_name, &m_value[0], 8);

			for (int j = i; j < 8; ++j)
			{
				s_global[j] = m_value[j];
			}
			return;
		}
	}
}

void MP_SpecularBlends8::apply(ShaderProgram* program)
{
	for (int i = 0; i < 8; ++i)
	{
		if (m_value[i] != s_global[i])
		{
			program->setArray(s_name, &m_value[0], 8);

			for (int j = i; j < 8; ++j)
			{
				s_global[j] = m_value[j];
			}
			return;
		}
	}
}

void MP_Diff7Spec7NormHeight::apply(ShaderProgram* program)
{
	if (s_first)
	{
		program->setArray(s_diff_name, &units[0], 7);
		program->setArray(s_spec_name, &units[7], 7);
		program->set(s_norm_name, 14);
		program->set(s_height_name, 15);
		s_first = false;
	}

	if (s_global_diff_count != m_diff_count)
	{
		program->set(s_diff_count_name, m_diff_count);
		s_global_diff_count = m_diff_count;
	}

	if (s_global_spec_count != m_spec_count)
	{
		program->set(s_spec_count_name, m_spec_count);
		s_global_spec_count = m_spec_count;
	}

	for (GLsizei i = 0; i < m_diff_count; ++i)
		m_diff[i]->bind(i); // 0Ц6

	for (GLsizei i = 0; i < m_spec_count; ++i)
		m_spec[i]->bind(i + 7); // 7Ц13

	if (m_normal)
		m_normal->bind(14); // нормаль

	if (m_height)
		m_height->bind(15); // высота
}

void MP_Diff7Spec7NormHeight::pushDiffuse(Texture* texture)
{
	if (m_diff_count == 7)
	{
		LOG_ERROR_S("diffuse textures count should be <= 7 in MP_Diff7Spec7Norm");
		return;
	}
	if (!texture)
	{
		LOG_ERROR_S("MP_Diff7Spec7Norm received invalid diffuse texture");
		return;
	}
	m_diff[m_diff_count++] = texture;
}

void MP_Diff7Spec7NormHeight::pushSpecular(Texture* texture)
{
	if (m_spec_count == 7)
	{
		LOG_ERROR_S("specular textures count should be <= 7 in MP_Diff7Spec7Norm");
		return;
	}
	if (!texture)
	{
		LOG_ERROR_S("MP_Diff7Spec7Norm received invalid specular texture");
		return;
	}
	m_spec[m_spec_count++] = texture;
}

void MP_Diff7Spec7NormHeight::popDiffuse()
{
	if (m_diff_count == 0)
	{
		LOG_ERROR_S("diffuse count already 0");
		return;
	}
	m_diff_count--;
}

void MP_Diff7Spec7NormHeight::popSpecular()
{
	if (m_spec_count == 0)
	{
		LOG_ERROR_S("diffuse count already 0");
		return;
	}
	m_spec_count--;
}

void MP_Diff7Spec7NormHeight::setNormal(Texture* texture)
{
	if (!texture)
	{
		LOG_ERROR_S("MP_Diff7Spec7Norm received invalid normal texture");
		return;
	}
	m_normal = texture;
}

void MP_Diff7Spec7NormHeight::setHeight(Texture* texture)
{
	if (!texture)
	{
		LOG_ERROR_S("MP_Diff7Spec7Norm received invalid height texture");
		return;
	}
	m_height = texture;
}

void MP_Diff7Spec7Norm::apply(ShaderProgram* program)
{
	if (s_first)
	{
		program->setArray(s_diff_name, &units[0], 7);
		program->setArray(s_spec_name, &units[7], 7);
		program->set(s_norm_name, 14);
		s_first = false;
	}

	if (s_global_diff_count != m_diff_count)
	{
		program->set(s_diff_count_name, m_diff_count);
		s_global_diff_count = m_diff_count;
	}

	if (s_global_spec_count != m_spec_count)
	{
		program->set(s_spec_count_name, m_spec_count);
		s_global_spec_count = m_spec_count;
	}

	for (GLsizei i = 0; i < m_diff_count; ++i)
		m_diff[i]->bind(i); // 0Ц6

	for (GLsizei i = 0; i < m_spec_count; ++i)
		m_spec[i]->bind(i + 7); // 7Ц13

	if (m_normal)
		m_normal->bind(14); // нормаль на 14-й юнит
}

void MP_Diff7Spec7Norm::pushDiffuse(Texture* texture)
{
	if (m_diff_count == 7)
	{
		LOG_ERROR_S("diffuse textures count should be <= 7 in MP_Diff7Spec7Norm");
		return;
	}
	if (!texture)
	{
		LOG_ERROR_S("MP_Diff7Spec7Norm received invalid diffuse texture");
		return;
	}
	m_diff[m_diff_count++] = texture;
}

void MP_Diff7Spec7Norm::popDiffuse()
{
	if (m_diff_count == 0)
	{
		LOG_ERROR_S("diffuse count already 0");
		return;
	}
	m_diff_count--;
}

void MP_Diff7Spec7Norm::pushSpecular(Texture* texture)

{
	if (m_spec_count == 7)
	{
		LOG_ERROR_S("specular textures count should be <= 7 in MP_Diff7Spec7Norm");
		return;
	}
	if (!texture)
	{
		LOG_ERROR_S("MP_Diff7Spec7Norm received invalid specular texture");
		return;
	}
	m_spec[m_spec_count++] = texture;
}

void MP_Diff7Spec7Norm::popSpecular()
{
	if (m_spec_count == 0)
	{
		LOG_ERROR_S("specular count already 0");
		return;
	}
	m_spec_count--;
}

void MP_Diff7Spec7Norm::setNormal(Texture* texture)
{
	if (!texture)
	{
		LOG_ERROR_S("MP_Diff7Spec7Norm received invalid normal texture");
		return;
	}
	m_normal = texture;
}

void MP_Diff8Spec8::apply(ShaderProgram* program)
{
	if (s_first)
	{
		program->setArray(s_diff_name, &units[0], 8);
		program->setArray(s_spec_name, &units[8], 8);
		s_first = false;
	}

	if (s_global_diff_count != m_diff_count)
	{
		program->set("material.diffuse_count", m_diff_count);
		s_global_diff_count = m_diff_count;
	}

	if (s_global_spec_count != m_spec_count)
	{
		program->set("material.specular_count", m_spec_count);
		s_global_spec_count = m_spec_count;
	}

	for (GLsizei i = 0; i < m_diff_count; ++i)
	{
		m_diff[i]->bind(i);
	}

	for (GLsizei i = 0; i < m_spec_count; ++i)
	{
		m_spec[i]->bind(i + 8);
	}
}

void MP_Diff8Spec8::pushDiffuse(Texture* texture)
{
	if (m_diff_count == 8)
	{
		LOG_ERROR_S("diffuse textures count should be <= 8 in MP_Diff8Spec8");
		return;
	}
	if (!texture)
	{
		LOG_ERROR_S("MP_Diffuse16 receive invalid diffuse texture");
		return;
	}

	m_diff[m_diff_count] = texture;
	m_diff_count++;
}

void MP_Diff8Spec8::popDiffuse()
{
	if (m_diff_count == 0)
	{
		LOG_ERROR_S("diffuse count already 0");
		return;
	}
	m_diff_count--;
}

void MP_Diff8Spec8::pushSpecular(Texture* texture)
{
	if (m_spec_count == 8)
	{
		LOG_ERROR_S("specular textures count should be <= 8 in MP_Diff8Spec8");
		return;
	}

	if (!texture)
	{
		LOG_ERROR_S("MP_Diff8Spec8 received invalid specular texture");
		return;
	}

	m_spec[m_spec_count] = texture;
	m_spec_count++;
}

void MP_Diff8Spec8::popSpecular()
{
	if (m_spec_count == 0)
	{
		LOG_ERROR_S("specular count already 0");
		return;
	}
	m_spec_count--;
}

GLsizei MP_Diff8Spec8::getSpecularCount()
{
	return m_spec_count;
}

GLsizei MP_Diff8Spec8::getDiffuseCount()
{
	return m_diff_count;
}

void MP_Diffuse16::apply(ShaderProgram* program)
{
	if (s_first)
	{
		program->setArray(s_name, &units[0], 16);
		s_first = false;
	}
	if (s_global_count != m_count)
	{
		program->set(s_count_name, m_count);
		s_global_count = m_count;
	}
	for (GLsizei i = 0; i < m_count; ++i)
	{
		m_value[i]->bind(i);
	}
}

void MP_Diffuse16::pushDiffuse(Texture* texture)
{
	if (m_count == 16)
	{
		LOG_ERROR_S("diffuse textures count should be <= 16 in MP_Diffuse16");
		return;
	}
	if (!texture)
	{
		LOG_ERROR_S("MP_Diffuse16 receive invalid diffuse texture");
		return;
	}

	m_value[m_count] = texture;
	m_count++;
}

void MP_Diffuse16::popDiffuse()
{
	if (m_count == 0)
	{
		LOG_ERROR_S("diffuse count already 0");
		return;
	}
	m_count--;
}

GLsizei MP_Diffuse16::getDiffuseCount()
{
	return m_count;
}
