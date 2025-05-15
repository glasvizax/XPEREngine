#include "Texture.h"

#ifdef _DEBUG
static std::string debug_name_default = "Texture";
static uint		   count = 0;

Texture::Texture(const std::string& debug_name)
	: m_debug_name(debug_name)
{
}

#endif // _DEBUG

Texture::Texture()
	:
#ifdef _DEBUG
	m_debug_name(debug_name_default + std::to_string(count))
#endif // _DEBUG
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

Texture::Texture(Texture&& other) noexcept
{
	this->~Texture();
	this->m_channels_num = other.m_channels_num;
#ifdef _DEBUG
	this->m_debug_name = std::move(other.m_debug_name);
#endif
	this->m_has_mipmap = other.m_has_mipmap;
	this->m_id = other.m_id;
	other.m_id = 0;
	this->m_height = other.m_height;
	this->m_width = other.m_width;
	this->m_initialized = other.m_initialized;
	other.m_initialized = false;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	this->~Texture();
	this->m_channels_num = other.m_channels_num;
#ifdef _DEBUG
	this->m_debug_name = std::move(other.m_debug_name);
#endif
	this->m_has_mipmap = other.m_has_mipmap;
	this->m_id = other.m_id;
	other.m_id = 0;
	this->m_height = other.m_height;
	this->m_width = other.m_width;
	this->m_initialized = other.m_initialized;
	other.m_initialized = false;

	return *this;
}

void Texture::init(int width, int height, GLint internal_format, uint channels_num, bool generate_mipmap)
{
	if (m_initialized)
	{
		LOG_ERROR_F("[%s] : Texture is already initialized", m_debug_name.c_str());
		return;
	}
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	if (generate_mipmap)
	{
		int levels = 1 + static_cast<int>(std::floor(std::log2(std::max(width, height))));
		glTextureStorage2D(m_id, levels, internal_format, width, height);
	}
	else
	{
		glTextureStorage2D(m_id, 1, internal_format, width, height);
	}
	
	checkGeneralErrorGL(m_debug_name);

	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkGeneralErrorGL(m_debug_name);

	m_width = width;
	m_height = height;

	m_has_mipmap = generate_mipmap;
	m_channels_num = channels_num;

	m_initialized = true;
}

void Texture::loadData(GLenum type, GLenum format, const void* data, glm::vec2 start_factors, glm::vec2 end_factors)
{
	if (!m_initialized)
	{
		LOG_ERROR_F("[%s] : Texture not initialized", m_debug_name.c_str());		
		return;
	}
	if (!data)
	{
		LOG_ERROR_F("[%s] : data invalid", m_debug_name.c_str());
		return;
	}

	int xoffset = static_cast<int>(m_width * start_factors.x);
	int yoffset = static_cast<int>(m_height * start_factors.y);
	int width = static_cast<int>(m_width * (end_factors.x - start_factors.x));
	int height = static_cast<int>(m_height * (end_factors.y - start_factors.y));
	
	if (xoffset < 0 || yoffset < 0 || width <= 0 || height <= 0 || xoffset + width > m_width || yoffset + height > m_height)
	{
		LOG_ERROR_F("[%s] : loadData region is out of bounds", m_debug_name.c_str());
		return;
	}

	glTextureSubImage2D(m_id, 0, xoffset, yoffset, width, height, format, type, data);
	checkGeneralErrorGL(m_debug_name);

	if (m_has_mipmap)
	{
		generateMipMap();
	}
}

void Texture::setMinFilter(GLint min_filter)
{
	if (!m_initialized)
	{
		LOG_ERROR_F("[%s] : Texture not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, min_filter);
	checkGeneralErrorGL(m_debug_name);
}

void Texture::setMagFilter(GLint mag_filter)
{
	if (!m_initialized)
	{
		LOG_ERROR_F("[%s] : Texture not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, mag_filter);
	checkGeneralErrorGL(m_debug_name);
}

void Texture::setWrapS(GLint wrap_s)
{
	if (!m_initialized)
	{
		LOG_ERROR_F("[%s] : Texture not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, wrap_s);
	checkGeneralErrorGL(m_debug_name);
}

void Texture::setWrapT(GLint wrap_t)
{
	if (!m_initialized)
	{
		LOG_ERROR_F("[%s] : Texture not initialized", m_debug_name.c_str());
		return;
	}
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, wrap_t);
	checkGeneralErrorGL(m_debug_name);
}

void Texture::bind(GLuint i)
{
	if (!m_initialized)
	{
		LOG_ERROR_F("[%s] : Texture not initialized", m_debug_name.c_str());
		return;
	}

	auto at_unit = s_bound.find(i);
	if (at_unit != s_bound.end())
	{
		if (at_unit->second == m_id)
		{
			return;
		}
	}

	s_bound[i] = m_id;
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, m_id);
	checkGeneralErrorGL(m_debug_name);
}

GLuint Texture::getID() const
{
	return m_id;
}

uint Texture::getChannelsNum() const
{
	if (!m_initialized)
	{
		LOG_ERROR_F("[%s] : Texture not initialized", m_debug_name.c_str());
		return 0;
	}
	return m_channels_num;
}

void Texture::generateMipMap()
{
	glGenerateTextureMipmap(m_id);
	checkGeneralErrorGL(m_debug_name);
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	checkGeneralErrorGL(m_debug_name);
}
