#include "VertexArray.h"

#include "DebugOpenGL.h"
#include "Defines.h"

#ifdef _DEBUG

static uint count = 0;

VertexArray::VertexArray()
	: m_debug_name("VertexArray" + std::to_string(count++)) {}

VertexArray::VertexArray(const std::string& debug_name)
	: m_debug_name(debug_name) {}

#else

VertexArray::VertexArray() {}

#endif

void VertexArray::init()
{
	if (m_id)
	{
		LOG_WARNING_F("[%s] : VertexArray is already initialized", m_debug_name.c_str());
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
	m_id = other.m_id;
	other.m_id = 0;

#ifdef _DEBUG
	m_debug_name = std::move(other.m_debug_name);
#endif

	m_buffers = std::move(other.m_buffers);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		clear();

		m_id = other.m_id;
		other.m_id = 0;

#ifdef _DEBUG
		m_debug_name = std::move(other.m_debug_name);
#endif

		m_buffers = std::move(other.m_buffers);
	}
	return *this;
}

VertexArray::~VertexArray()
{
	clear();
}

void VertexArray::bind()
{
	if (!m_id)
	{
		LOG_ERROR_F("[%s] : VertexArray not initialized", m_debug_name.c_str());
		return;
	}
	if (s_bound_id == m_id)
	{
		return;
	}
	s_bound_id = m_id;
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

GLuint VertexArray::getID()
{
	return m_id;
}

void VertexArray::clear()
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
