#include "Framebuffer.h"

#include <iostream>

#include "DebugOpenGL.h"

// clang-format off
Framebuffer::Framebuffer
(
#ifdef _DEBUG
	const std::string& debug_name
#endif
) :
#ifdef _DEBUG
	m_debug_name(debug_name)
#endif
{
	glGenFramebuffers(1, &m_id);
}
// clang-format on

Framebuffer::~Framebuffer()
{
	remove();
}

void Framebuffer::remove()
{
	if (m_id)
	{
		if (s_active_id == m_id)
		{
			unbind();
		}
		glDeleteFramebuffers(1, &m_id);
		m_id = 0;
	}

	// checkGeneralErrorGL(m_debug_name);
	// checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::attachTexture2D(GLuint tex_id, AttachmentType type)
{
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(type), GL_TEXTURE_2D, tex_id, 0);

	checkGeneralErrorGL(m_debug_name); 
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::attachRenderbuffer(GLuint rbo_id, AttachmentType type)
{
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(type), GL_RENDERBUFFER, rbo_id);

	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::bind()
{
	if (s_active_id == m_id)
	{
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	s_active_id = m_id;

	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::unbind()
{
	if (s_active_id == m_id)
	{
		s_active_id = 0;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::drawBuffers(GLuint* attachments, GLsizei size)
{
	bind();
	glDrawBuffers(size, attachments);

	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::drawBuffer(GLBuffer buffer)
{
	bind();
	glDrawBuffer(static_cast<GLenum>(buffer));

	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::readBuffer(GLBuffer buffer)
{
	bind();
	glReadBuffer(static_cast<GLenum>(buffer));

	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}
