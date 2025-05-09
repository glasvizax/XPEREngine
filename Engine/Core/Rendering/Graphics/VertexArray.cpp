#include "VertexArray.h"

#include "DebugOpenGL.h"

// clang-format off
VertexArray::VertexArray(
#ifdef _DEBUG
	const std::string& debug_name
#endif
) :
#ifdef _DEBUG
	m_debug_name(debug_name)
#endif
{
	glGenVertexArrays(1, &m_id);
	checkGeneralErrorGL(m_debug_name);
}
// clang-format on

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::attachBuffer(BufferType type, size_t size, DrawMode mode, const void* data) 
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(type, buffer);
	checkGeneralErrorGL(m_debug_name);
	glBufferData(type, size, data, mode);
	checkGeneralErrorGL(m_debug_name);
}

void VertexArray::bind()
{
	glBindVertexArray(m_id);
	checkGeneralErrorGL(m_debug_name);
}

void VertexArray::enableAttribute(GLuint index, GLint size, GLuint offset, const void* data)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, offset, data);
	checkGeneralErrorGL(m_debug_name);
}