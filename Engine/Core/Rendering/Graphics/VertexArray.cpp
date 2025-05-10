#include "VertexArray.h"

#include "DebugOpenGL.h"
#include "Defines.h"

static uint count = 0;

// clang-format off
VertexArray::VertexArray()
#ifdef _DEBUG
	: m_debug_name("VAO" + std::to_string(count++)) 
#endif
{}

VertexArray::VertexArray(
#ifdef _DEBUG
	const std::string& debug_name
#endif
) :
#ifdef _DEBUG
	m_debug_name(debug_name)
#endif
{}

void VertexArray::init()
{
	glGenVertexArrays(1, &m_id);
	checkGeneralErrorGL(m_debug_name);
}
// clang-format on

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::attachBuffer(GLsizeiptr size, const void* data, BufferType type, DrawMode mode)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(scast<GLenum>(type), buffer);
	checkGeneralErrorGL(m_debug_name);

	glBufferData(scast<GLenum>(type), size, data, scast<GLenum>(mode));
	checkGeneralErrorGL(m_debug_name);
}

void VertexArray::bind()
{
	glBindVertexArray(m_id);
	checkGeneralErrorGL(m_debug_name);
}

void VertexArray::enableAttribute(GLuint index, GLint components_num, GLuint stride_count, GLsizei offset)
{
	glEnableVertexAttribArray(index);

	glVertexAttribPointer(index, components_num, GL_FLOAT, GL_FALSE, stride_count * sizeof(float), rcast<void*>(sizeof(float) * offset));
	checkGeneralErrorGL(m_debug_name);
}