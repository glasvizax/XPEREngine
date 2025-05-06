#include "Shader.h"

bool setupShader(const char* const src, GLenum shader_type, GLuint& result) 
{
	result = glCreateShader(shader_type);
	glShaderSource(result, 1, &src, nullptr);

	glCompileShader(result);

	GLint is_compiled;
	glGetShaderiv(result, GL_COMPILE_STATUS, &is_compiled);
	if (is_compiled == GL_FALSE)
	{
		GLint log_length;
		glGetShaderiv(result, GL_INFO_LOG_LENGTH, &log_length);

		gl_string log;
		log.resize(log_length);
		glGetShaderInfoLog(result, log_length, nullptr, log.data());

		LOG_ERROR(log);

		glDeleteShader(result);
		return false;
	}
	return true;
}

bool setupProgram(GLuint* shaders, uint count, GLuint& program)
{
	program = glCreateProgram();
	
	for (uint i = 0; i < count; ++i)
	{
		glAttachShader(program, shaders[i]);	
	}
	glLinkProgram(program);

	GLint is_linked;
	glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
	if (is_linked == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		gl_string log;
		log.resize(length);
		glGetProgramInfoLog(program, length, nullptr, log.data());
		LOG_ERROR(log);

		glDeleteProgram(program);

		for (uint i = 0; i < count; ++i)
		{
			glDeleteShader(shaders[i]);
		}

		return false;
	}

	for (uint i = 0; i < count; ++i)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	return true;
}

Shader::Shader(const char* const vertex_src, const char* const fragment_src)
{
	if (!vertex_src)
	{
		LOG_ERROR("Invalid vertex source");
		return;
	}
	if (!fragment_src)
	{
		LOG_ERROR("Invalid fragment source");
		return;
	}

	GLuint shaders[2];

	if (!setupShader(vertex_src, GL_VERTEX_SHADER, shaders[0]))
	{
		return;
	}

	if (!setupShader(fragment_src, GL_FRAGMENT_SHADER, shaders[1]))
	{
		glDeleteShader(shaders[0]);
		return;
	}
	
	GLuint program;

	if (!setupProgram(shaders, 2, program))
	{
		return;
	}

	m_program_id = program;
}

Shader::Shader(const char* vertex_src, const char* fragment_src, const char* geometry_src)
{
	if (!vertex_src)
	{
		LOG_ERROR("Invalid vertex source");
		return;
	}
	if (!fragment_src)
	{
		LOG_ERROR("Invalid fragment source");
		return;
	}
	if (!geometry_src)
	{
		LOG_ERROR("Invalid geometry source");
		return;
	}

	GLuint shaders[3];
	if (!setupShader(vertex_src, GL_VERTEX_SHADER, shaders[0]))
	{
		return;
	}

	if (!setupShader(fragment_src, GL_FRAGMENT_SHADER, shaders[1]))
	{
		glDeleteShader(shaders[0]);
		return;
	}

	if (!setupShader(geometry_src, GL_GEOMETRY_SHADER, shaders[2]))
	{
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		return;
	}

	GLuint program;

	if (!setupProgram(shaders, 3, program))
	{
		return;
	}

	m_program_id = program;
}
