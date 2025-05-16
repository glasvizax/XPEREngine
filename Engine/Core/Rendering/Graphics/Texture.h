#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "DebugOpenGL.h"
#include "Defines.h"
#include "Debug.h"

class Texture
{
public:
	Texture();

#ifdef _DEBUG
	Texture(const std::string& debug_name);
#endif

	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void init(int width, int height, GLint internal_format, uint channels_num, bool generate_mipmap);

	void loadData(GLenum type, GLenum format, const void* data, glm::vec2 start_factors = glm::vec2(0.0f), glm::vec2 end_factors = glm::vec2(1.0f));

	void setMinFilter(GLint min_filter);

	void setMagFilter(GLint mag_filter);

	void setWrapS(GLint wrap_s);

	void setWrapT(GLint wrap_t);

	void bind(GLuint i = 0);

	GLuint getID() const;

	uint getChannelsNum() const;

private:
	bool m_has_mipmap = false;

	GLsizei m_height;
	GLsizei m_width;

	GLuint m_id = 0;
	uint   m_channels_num;

private:
	void generateMipMap();
	void clear();

	inline static std::unordered_map<uint8_t, GLuint> s_bound;

#ifdef _DEBUG
	std::string m_debug_name;
#endif // _DEBUG
};