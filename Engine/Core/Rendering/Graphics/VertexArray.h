#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

class VertexArray
{
	friend class RenderSystem;

public:
	VertexArray();

#ifdef _DEBUG
	VertexArray(const std::string& debug_name);
	std::string m_debug_name;
#endif

	VertexArray(const VertexArray& other) = delete;
	VertexArray& operator=(const VertexArray& other) = delete;

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	~VertexArray();

	void init();

	void attachArrayBuffer(GLsizeiptr bytes, const void* data, GLenum draw_mode = GL_STATIC_DRAW);
	void attachElementBuffer(GLsizeiptr bytes, const void* data, GLenum draw_mode = GL_STATIC_DRAW);

	void bind();

	template <typename VertexType>
	void autoEnableAttributes();

	void enableAttribute(GLuint index, GLint components_num, GLuint stride_count, GLsizei offset);

	GLuint getID();

private:
	void clear();

	GLuint				m_id = 0;
	std::vector<GLuint> m_buffers;

	inline static GLuint s_bound_id = -1;
};
