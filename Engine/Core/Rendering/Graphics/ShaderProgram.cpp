#include "ShaderProgram.h"

#include "Defines.h"

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

		LOG_ERROR_S(log.c_str());

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
		LOG_ERROR_S(log.c_str());

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

#ifdef _DEBUG
static uint count = 0;
#endif

ShaderProgram::ShaderProgram()
#ifdef _DEBUG
	: m_debug_name("ShaderProgram" + std::to_string(count++))
#endif
{
}

#ifdef _DEBUG
ShaderProgram::ShaderProgram(const std::string& debug_name)
	: m_debug_name(debug_name) {}
#endif

bool ShaderProgram::init(const char* const vertex_src, const char* const fragment_src)
{
	if (!vertex_src)
	{
		LOG_ERROR_S("Invalid vertex source");
		return false;
	}
	if (!fragment_src)
	{
		LOG_ERROR_S("Invalid fragment source");
		return false;
	}

	GLuint shaders[2];

	if (!setupShader(vertex_src, GL_VERTEX_SHADER, shaders[0]))
	{
		return false;
	}
	if (!setupShader(fragment_src, GL_FRAGMENT_SHADER, shaders[1]))
	{
		glDeleteShader(shaders[0]);
		return false;
	}

	GLuint program;

	if (!setupProgram(shaders, 2, program))
	{
		return false;
	}

	m_program_id = program;
	return true;
}

bool ShaderProgram::init(const char* const vertex_src, const char* const fragment_src, const char* const geometry_src) 
{
	if (!vertex_src)
	{
		LOG_ERROR_S("Invalid vertex source");
		return false;
	}
	if (!fragment_src)
	{
		LOG_ERROR_S("Invalid fragment source");
		return false;
	}
	if (!geometry_src)
	{
		LOG_ERROR_S("Invalid geometry source");
		return false;
	}

	GLuint shaders[3];
	if (!setupShader(vertex_src, GL_VERTEX_SHADER, shaders[0]))
	{
		return false;
	}

	if (!setupShader(fragment_src, GL_FRAGMENT_SHADER, shaders[1]))
	{
		glDeleteShader(shaders[0]);
		return false;
	}

	if (!setupShader(geometry_src, GL_GEOMETRY_SHADER, shaders[2]))
	{
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		return false;
	}

	GLuint program;

	if (!setupProgram(shaders, 3, program))
	{
		return false;
	}

	m_program_id = program;
	return true;

}

void ShaderProgram::use() const
{
	if (m_active_program == m_program_id)
	{
		return;
	}
	glUseProgram(m_program_id);
	m_active_program = m_program_id;
}
