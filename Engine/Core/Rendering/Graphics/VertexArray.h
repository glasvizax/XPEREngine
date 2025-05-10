#pragma once

#include <string>
#include <glad/glad.h>

enum class BufferType : GLenum
{
	ARRAY = GL_ARRAY_BUFFER,
	ELEMENT = GL_ELEMENT_ARRAY_BUFFER
};

enum class DrawMode : GLenum
{
	STATIC = GL_STATIC_DRAW,
	DYNAMIC = GL_DYNAMIC_DRAW,
	STREAM = GL_STREAM_DRAW
};

class VertexArray
{
public:

	VertexArray();

	VertexArray
	(
#ifdef _DEBUG
		const std::string& debug_name
#endif
	);

	~VertexArray();

	void init();

	void attachBuffer(GLsizeiptr size, const void* data, BufferType type, DrawMode mode = DrawMode::STATIC);
	void bind();

	/// <summary>
	/// enables vertex attribute
	/// </summary>
	/// <param name="index - attribute index"></param>
	/// <param name="components_num - amount of elements in attribute "></param>
	/// <param name="stride_count - number (in count) of elements until the next same attribute"></p
	/// <param name="offset - offset (in count) of this attribute from the start"></param>
	void enableAttribute(GLuint index, GLint components_num, GLuint stride_count, GLsizei offset);

private:
	GLuint m_id = 0;

	#ifdef _DEBUG
	std::string m_debug_name;
	#endif
};