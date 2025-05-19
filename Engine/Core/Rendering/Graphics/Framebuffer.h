#pragma once

#include <string>

#include <glad/glad.h>

#include <Defines.h>

class Framebuffer
{
public:
	Framebuffer();

#ifdef _DEBUG
	Framebuffer(const std::string& debug_name);
	std::string m_debug_name;
#endif

	Framebuffer(const Framebuffer& other) = delete;
	Framebuffer& operator=(const Framebuffer& other) = delete;
	Framebuffer(Framebuffer&& other) noexcept = delete;
	Framebuffer& operator=(Framebuffer&& other) noexcept = delete;

	~Framebuffer();

	void init();
	void clear();

	void attachTexture2D(GLuint tex_id, GLenum attachment_type = GL_COLOR_ATTACHMENT0);
	void attachRenderbuffer(GLuint rbo_id, GLenum attachment_type = GL_DEPTH_STENCIL_ATTACHMENT);
	void createAttachRenderbuffer(GLsizei width, GLsizei height, GLenum internal_format = GL_DEPTH24_STENCIL8, GLenum attachment_type = GL_DEPTH_STENCIL_ATTACHMENT);

	void bind();
	static void bindDefault();

	void drawBuffersDefault(GLsizei size);
	void drawBuffers(GLenum* buffers, GLsizei size);

private:
	GLuint				 m_id = 0u;
	GLuint				 m_rbo_id = 0u;
	bool				 m_owns_rbo = false;
	inline static GLuint s_active_id = 0;

	static GLenum s_default_draw_buffers[16];
};
