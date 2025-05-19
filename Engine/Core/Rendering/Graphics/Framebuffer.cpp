#include "Framebuffer.h"

#include <iostream>

#include "DebugOpenGL.h"

#ifdef _DEBUG

static uint count = 0;

Framebuffer::Framebuffer()
	: m_debug_name("Framebuffer" + std::to_string(count++)) {}

Framebuffer::Framebuffer(const std::string& debug_name) 
	: m_debug_name(debug_name)
{
	
}

#else 

Framebuffer::Framebuffer() {}

#endif

GLenum Framebuffer::s_default_draw_buffers[16] = {
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,
	GL_COLOR_ATTACHMENT9,
	GL_COLOR_ATTACHMENT10,
	GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12,
	GL_COLOR_ATTACHMENT13,
	GL_COLOR_ATTACHMENT14,
	GL_COLOR_ATTACHMENT15
};

Framebuffer::~Framebuffer()
{
	clear();
}

void Framebuffer::init()
{
	glGenFramebuffers(1, &m_id);
}

void Framebuffer::clear()
{
	if (m_id)
	{
		glDeleteFramebuffers(1, &m_id);
		m_id = 0;
	}

	if (m_owns_rbo && m_rbo_id != 0)
	{
		glDeleteRenderbuffers(1, &m_rbo_id);
		m_rbo_id = 0;
		m_owns_rbo = false;
	}
}

void Framebuffer::attachTexture2D(GLuint tex_id, GLenum attachment_type)
{
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, GL_TEXTURE_2D, tex_id, 0);

	checkGeneralErrorGL(m_debug_name); 
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::attachRenderbuffer(GLuint rbo_id, GLenum attachment_type)
{
	bind();
	if (m_owns_rbo && m_rbo_id != 0)
	{
		glDeleteRenderbuffers(1, &m_rbo_id);
		m_owns_rbo = false;
		m_rbo_id = 0;
	}
	m_rbo_id = rbo_id;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_type, GL_RENDERBUFFER, rbo_id);
	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::createAttachRenderbuffer(GLsizei width, GLsizei height, GLenum internal_format, GLenum attachment_type)
{
	bind();
	
	if (m_owns_rbo && m_rbo_id != 0)
	{
		glDeleteRenderbuffers(1, &m_rbo_id);
	}

	glGenRenderbuffers(1, &m_rbo_id);
	m_owns_rbo = true; 
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_id);
	glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_type, GL_RENDERBUFFER, m_rbo_id);
}

void Framebuffer::bind()
{
	if (s_active_id == m_id)
	{
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	s_active_id = m_id;
}

void Framebuffer::bindDefault()
{
	s_active_id = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	checkGeneralErrorGL("default_framebuffer");
	checkFramebufferErrorsGL("default_framebuffer");
}

void Framebuffer::drawBuffersDefault(GLsizei size)
{
	bind();
	if (size > 16)
	{
		LOG_WARNING_S("size shoudld be <= 16");
		size = 16;
	}

	glDrawBuffers(size, s_default_draw_buffers);

	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);

	LOG_INFO_S("void Framebuffer::drawBuffersDefault(GLsizei size)");
}
void Framebuffer::drawBuffers(GLenum* buffers, GLsizei size)
{
	glDrawBuffers(size, buffers);
	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}



/*
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
*/