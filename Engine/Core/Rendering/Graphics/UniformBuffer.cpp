#include "UniformBuffer.h"
#include "Defines.h"
#include "Debug.h"
#include "DebugOpenGL.h"

#ifdef _DEBUG

static uint count = 0;

UniformBuffer::UniformBuffer()
	: m_debug_name("UniformBuffer" + std::to_string(count++)) {}

UniformBuffer::UniformBuffer(const std::string& debug_name)
	: m_debug_name(debug_name) {}

#else

UniformBuffer::UniformBuffer() {}

#endif

UniformBuffer::~UniformBuffer()
{
	clear();
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
{
	m_id = other.m_id;
	other.m_id = 0;
}
UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
{
	if (&other != this)
	{
		clear();
		m_id = other.m_id;
		other.m_id = 0;
	}
	return *this;
}
void UniformBuffer::init(GLsizei bytes, GLuint index, GLenum usage)
{
	if (m_id)
	{
		LOG_WARNING_F("[%s] : UniformBuffer is already initialized", m_debug_name.c_str());
		clear();
	}
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferData(GL_UNIFORM_BUFFER, bytes, nullptr, usage);
	glBindBufferBase(GL_UNIFORM_BUFFER, index, m_id);
	checkGeneralErrorGL(m_debug_name);
}

void UniformBuffer::bind()
{
	if (s_bound_id == m_id)
	{
		return;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	s_bound_id = m_id;
	checkGeneralErrorGL(m_debug_name);
}

void UniformBuffer::fill(GLintptr offset, GLsizeiptr size, const void* data)
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	checkGeneralErrorGL(m_debug_name);
}

void UniformBuffer::clear()
{
	if (m_id)
	{
		glDeleteBuffers(1, &m_id);
	}
}
