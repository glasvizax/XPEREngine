#pragma once

#include <string>

#include <glad/glad.h>

class UniformBuffer
{
public:

#ifdef _DEBUG
	UniformBuffer(const std::string& debug_name);
	std::string m_debug_name;
#endif

	UniformBuffer();
	~UniformBuffer();

	UniformBuffer(const UniformBuffer&) = delete;
	UniformBuffer& operator=(const UniformBuffer&) = delete;

	UniformBuffer(UniformBuffer&& other) noexcept;
	UniformBuffer& operator=(UniformBuffer&& other) noexcept;

	void init(GLsizei bytes, GLuint index, GLenum usage = GL_STATIC_DRAW);

	void bind();

	void fill(GLintptr offset, GLsizeiptr size, const void* data);

	void clear();

private:
	GLuint				 m_id = 0u;
	inline static GLuint s_bound_id = -1;
};
