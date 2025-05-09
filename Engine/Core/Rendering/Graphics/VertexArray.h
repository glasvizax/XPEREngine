#pragma once

#include <string>
#include <glad/glad.h>

class VertexArray
{
public:

	VertexArray
	(
#ifdef _DEBUG
		const std::string& debug_name
#endif
	
	);
	~VertexArray();

	enum BufferType : int
	{
		ARRAY = GL_ARRAY_BUFFER,
		ELEMENT = GL_ELEMENT_ARRAY_BUFFER
	};

	enum DrawMode : int
	{
		STATIC = GL_STATIC_DRAW,
		DYNAMIC = GL_DYNAMIC_DRAW,
		STREAM = GL_STREAM_DRAW
	};

	void attachBuffer(BufferType type, size_t size, DrawMode mode, const void* data);
	void bind();
	void enableAttribute(GLuint index, GLint size, GLuint offset, const void* data);

private:
	GLuint m_id = 0;

	#ifdef _DEBUG
	std::string m_debug_name;
	#endif
};