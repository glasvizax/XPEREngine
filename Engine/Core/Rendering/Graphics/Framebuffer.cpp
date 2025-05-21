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

	m_renderbuffer.clear();
}

void Framebuffer::attachTexture2D(Texture& texture, GLenum attachment_type)
{
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, GL_TEXTURE_2D, texture.getID(), 0);

	checkGeneralErrorGL(m_debug_name); 
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::attachRenderbuffer(Renderbuffer&& renderbuffer, GLenum attachment_type)
{
	bind();
	m_renderbuffer = std::move(renderbuffer);
	m_renderbuffer.bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_type, GL_RENDERBUFFER, m_renderbuffer.getID());
	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}

void Framebuffer::createAttachRenderbuffer(GLsizei width, GLsizei height, GLenum internal_format, GLenum attachment_type)
{
	bind();
	m_renderbuffer.init();
	m_renderbuffer.setStorage(width, height, internal_format);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_type, GL_RENDERBUFFER, m_renderbuffer.getID());
}

Renderbuffer& Framebuffer::getRenderbuffer()
{
	return m_renderbuffer;
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
}
void Framebuffer::drawBuffers(GLenum* buffers, GLsizei size)
{
	glDrawBuffers(size, buffers);
	checkGeneralErrorGL(m_debug_name);
	checkFramebufferErrorsGL(m_debug_name);
}

void Renderbuffer::init()
{
	glCreateRenderbuffers(1, &m_id);
}

Renderbuffer::Renderbuffer(Renderbuffer&& other) noexcept
	: m_id(other.m_id)
{
	other.m_id = 0;
}

Renderbuffer& Renderbuffer::operator=(Renderbuffer&& other) noexcept
{
	if (this != &other)
	{
		clear();
		m_id = other.m_id;
		other.m_id = 0;
	}
	return *this;
}

Renderbuffer::~Renderbuffer()
{
	clear();
}

void Renderbuffer::bind()
{
	if (s_active_id == m_id)
	{
		return;
	}
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	s_active_id = m_id;
}

void Renderbuffer::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::setStorage(GLsizei width, GLsizei height, GLenum internalformat)
{
	if (m_id == 0)
	{
		LOG_ERROR_S("m_id was 0");
	}
	bind();
	glNamedRenderbufferStorage(m_id, internalformat, width, height);
}

GLuint Renderbuffer::getID() const
{
	return m_id;
}

void Renderbuffer::clear()
{
	if (m_id != 0)
	{
		glDeleteRenderbuffers(1, &m_id);
	}
}
