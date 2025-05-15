#pragma once

#include <string>

#include <glad/glad.h>

#include <Defines.h>

class Framebuffer
{
public:
	Framebuffer(
#ifdef _DEBUG
		const std::string& debug_name
#endif
	);

	Framebuffer(Framebuffer&& other) noexcept = delete;
	Framebuffer& operator=(Framebuffer && other) noexcept = delete;

	~Framebuffer();

	enum class AttachmentType : GLenum
	{
		COLOR0 = GL_COLOR_ATTACHMENT0,
		COLOR1 = GL_COLOR_ATTACHMENT1,
		COLOR2 = GL_COLOR_ATTACHMENT2,
		COLOR3 = GL_COLOR_ATTACHMENT3,
		DEPTH = GL_DEPTH_ATTACHMENT,
		STENCIL = GL_STENCIL_ATTACHMENT
	};

	enum class BufferBitMasks : GLenum
	{
		COLOR = GL_COLOR_BUFFER_BIT,
		DEPTH = GL_DEPTH_BUFFER_BIT,
		STENCIL = GL_STENCIL_BUFFER_BIT
	};

	enum class GLBuffer : GLenum
	{
		NONE = GL_NONE,
		FRONT = GL_FRONT,
		BACK = GL_BACK,
		LEFT = GL_LEFT,
		RIGHT = GL_RIGHT,
		FRONT_RIGHT = GL_FRONT_RIGHT,
		BACK_LEFT = GL_BACK_LEFT
	};

	void remove();

	void attachTexture2D(GLuint tex_id, AttachmentType colorAttach);
	void attachRenderbuffer(GLuint rbo_id, AttachmentType type);

	void bind();
	void unbind();

	void drawBuffers(GLuint* attachments, GLsizei size);

	void drawBuffer(GLBuffer buffer);
	void readBuffer(GLBuffer buffer);

private:

#ifdef _DEBUG
	std::string m_debug_name;
#endif	
	GLuint m_id = GLuint(0);
	inline static GLuint s_active_id = GLuint(0);
};