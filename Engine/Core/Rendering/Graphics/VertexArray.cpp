#include "VertexArray.h"

#include "DebugOpenGL.h"
#include "Defines.h"

#ifdef _DEBUG
static uint count = 0;
#endif

VertexArray::VertexArray()
#ifdef _DEBUG
	: m_debug_name("VAO" + std::to_string(count++))
#endif
{
}

#ifdef _DEBUG
VertexArray::VertexArray(const std::string& debug_name)
	: m_debug_name(debug_name) {}
#endif

void VertexArray::init()
{
	if (m_id)
	{
		LOG_WARNING_S("void VertexArray::init() was called twice");
		glDeleteVertexArrays(1, &m_id);
	}
	glGenVertexArrays(1, &m_id);
}

void VertexArray::attachArrayBuffer(GLsizeiptr bytes, const void* data, GLenum draw_mode)
{
	bind();
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_buffers.push_back(vbo);
	glBufferData(GL_ARRAY_BUFFER, bytes, data, draw_mode);
	checkGeneralErrorGL(m_debug_name);
}

void VertexArray::attachElementBuffer(GLsizeiptr bytes, const void* data, GLenum draw_mode)
{
	bind();
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	m_buffers.push_back(ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes, data, draw_mode);
	checkGeneralErrorGL(m_debug_name);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
	this->~VertexArray();
	m_id = other.m_id;
	other.m_id = 0;

#ifdef _DEBUG
	m_debug_name = std::move(other.m_debug_name);
#endif

	m_buffers = std::move(other.m_buffers);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	this->~VertexArray();
	m_id = other.m_id;
	other.m_id = 0;

#ifdef _DEBUG
	m_debug_name = std::move(other.m_debug_name);
#endif

	m_buffers = std::move(other.m_buffers);

	return *this;
}

VertexArray::~VertexArray()
{
	if (m_id)
	{
		glDeleteVertexArrays(1, &m_id);
	}

	for (GLuint buffer : m_buffers)
	{
		glDeleteBuffers(1, &buffer);
	}
}

void VertexArray::bind()
{
	if (s_bounded_id == m_id)
	{
		return;
	}
	s_bounded_id = m_id;
	glBindVertexArray(m_id);
	checkGeneralErrorGL(m_debug_name);
}

void VertexArray::enableAttribute(GLuint index, GLint components_num, GLuint stride_count, GLsizei offset)
{
	bind();
	glEnableVertexAttribArray(index);

	glVertexAttribPointer(index, components_num, GL_FLOAT, GL_FALSE, stride_count * sizeof(float), rcast<void*>(sizeof(float) * offset));
	checkGeneralErrorGL(m_debug_name);
}