#include "Cubemap.h"

#ifdef _DEBUG
static uint count = 0;
#endif

#ifdef _DEBUG
Cubemap::Cubemap(const std::string& debug_name)
	: m_debug_name(debug_name) {}

Cubemap::Cubemap()
	: m_debug_name("Cubemap" + std::to_string(count++)) {}
#else
Cubemap::Cubemap() = default;
#endif

extern std::vector<GLuint> g_texture_units_bindings;

Cubemap::~Cubemap()
{
	clear();
}

Cubemap::Cubemap(Cubemap&& other) noexcept
{
	m_size = other.m_size;
	m_channels_num = other.m_channels_num;
	m_has_mipmap = other.m_has_mipmap;
	m_id = other.m_id;
	other.m_id = 0;
#ifdef _DEBUG
	m_debug_name = std::move(other.m_debug_name);
#endif

}

Cubemap& Cubemap::operator=(Cubemap&& other) noexcept
{
	if (this != &other)
	{
		clear();
		m_size = other.m_size;
		m_channels_num = other.m_channels_num;
		m_has_mipmap = other.m_has_mipmap;
		m_id = other.m_id;
		other.m_id = 0;
#ifdef _DEBUG
		m_debug_name = std::move(other.m_debug_name);
#endif
	}
	return *this;
}

void Cubemap::init(int size, GLint internal_format, uint channels_num, bool generate_mipmap)
{
	if (m_id)
	{
		LOG_ERROR_F("[%s] Already initialized", m_debug_name.c_str());
		return;
	}

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);
	checkGeneralErrorGL(m_debug_name);

	int levels = generate_mipmap ? 1 + static_cast<int>(std::log2(size)) : 1;
	glTextureStorage2D(m_id, levels, internal_format, size, size);
	checkGeneralErrorGL(m_debug_name);

	// Default parameters
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, generate_mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	checkGeneralErrorGL(m_debug_name);

	m_size = size;
	m_channels_num = channels_num;
	m_has_mipmap = generate_mipmap;
}

void Cubemap::loadFaceData(uint face_index, GLenum type, GLenum format, const void* data,
	glm::vec2 start_factors, glm::vec2 end_factors)
{
	if (!data)
	{
		LOG_ERROR_F("[%s] Null data pointer for face %d", m_debug_name.c_str(), face_index);
		return;
	}
	if (!m_id)
	{
		LOG_ERROR_F("[%s] Not initialized", m_debug_name.c_str());
		return;
	}
	if (face_index >= 6)
	{
		LOG_ERROR_F("[%s] Invalid face index: %d", m_debug_name.c_str(), face_index);
		return;
	}

	int xoffset = static_cast<int>(m_size * start_factors.x);
	int yoffset = static_cast<int>(m_size * start_factors.y);
	int width = static_cast<int>(m_size * (end_factors.x - start_factors.x));
	int height = static_cast<int>(m_size * (end_factors.y - start_factors.y));

	if (xoffset < 0 || yoffset < 0 || width <= 0 || height <= 0 || xoffset + width > m_size || yoffset + height > m_size)
	{
		LOG_ERROR_F("[%s] Invalid region for face %d", m_debug_name.c_str(), face_index);
		return;
	}

	glTextureSubImage3D(
		m_id, 0,
		xoffset, yoffset, face_index,
		width, height, 1,
		format, type, data);
	checkGeneralErrorGL(m_debug_name);

	if (m_has_mipmap)
	{
		generateMipMap();
	}
}

void Cubemap::setMinFilter(GLint min_filter)
{
	if (!m_id)
	{
		LOG_ERROR_F("[%s] Not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, min_filter);
}

void Cubemap::setMagFilter(GLint mag_filter)
{
	if (!m_id)
	{
		LOG_ERROR_F("[%s] Not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void Cubemap::setWrapS(GLint wrap_s)
{
	if (!m_id)
	{
		LOG_ERROR_F("[%s] Not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, wrap_s);
}

void Cubemap::setWrapT(GLint wrap_t)
{
	if (!m_id)
	{
		LOG_ERROR_F("[%s] Not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, wrap_t);
}

void Cubemap::setWrapR(GLint wrap_r)
{
	if (!m_id)
	{
		LOG_ERROR_F("[%s] Not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, wrap_r);
}

void Cubemap::bind(GLuint unit)
{
	if (!m_id)
	{
		LOG_ERROR_F("[%s] Not initialized", m_debug_name.c_str());
		return;
	}

	if (g_texture_units_bindings[unit] == m_id)
	{
		return;
	}

	g_texture_units_bindings[unit] = m_id;
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}

GLuint Cubemap::getID() const
{
	return m_id;
}

uint Cubemap::getChannelsNum() const
{
	if (!m_id)
	{
		LOG_ERROR_F("[%s] Not initialized", m_debug_name.c_str());
		return 0;
	}
	return m_channels_num;
}

void Cubemap::generateMipMap()
{
	if (!m_has_mipmap)
	{
		return;
	}
	glGenerateTextureMipmap(m_id);
	checkGeneralErrorGL(m_debug_name);
}

void Cubemap::clear()
{
	if (m_id)
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
}