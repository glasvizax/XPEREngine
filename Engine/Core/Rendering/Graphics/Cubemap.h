#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>

#include "DebugOpenGL.h"
#include "Defines.h"
#include "Debug.h"

#include "xm/xm.h"

class Cubemap
{
public:
	Cubemap();

#ifdef _DEBUG
	Cubemap(const std::string& debug_name);
#endif

	~Cubemap();

	Cubemap(const Cubemap&) = delete;
	Cubemap& operator=(const Cubemap&) = delete;

	Cubemap(Cubemap&& other) noexcept;
	Cubemap& operator=(Cubemap&& other) noexcept;

	void init(int size, GLint internal_format, uint channels_num, bool generate_mipmap);
	void loadFaceData(uint face_index, GLenum type, GLenum format, const void* data,
		xm::vec2 start_factors = xm::vec2(0.0f),
		xm::vec2 end_factors = xm::vec2(1.0f));

	void setMinFilter(GLint min_filter);
	void setMagFilter(GLint mag_filter);
	void setWrapS(GLint wrap_s);
	void setWrapT(GLint wrap_t);
	void setWrapR(GLint wrap_r);

	void	bind(GLuint unit = 0);
	GLuint	getID() const;
	uint	getChannelsNum() const;
	GLsizei getSize() const;

private:
	bool	m_has_mipmap = false;
	GLsizei m_size;
	GLuint	m_id = 0;
	uint	m_channels_num;

	void generateMipMap();
	void clear();

#ifdef _DEBUG
	std::string m_debug_name;
#endif
};