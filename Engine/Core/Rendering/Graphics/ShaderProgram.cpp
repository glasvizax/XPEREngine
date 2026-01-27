#include "ShaderProgram.h"

#include "Defines.h"

/********************DEBUG**********************/
#ifdef _DEBUG

static uint count = 0;

ShaderProgram::ShaderProgram()
	: m_debug_name("ShaderProgram" + std::to_string(count++)) {}

ShaderProgram::ShaderProgram(const std::string& debug_name)
	: m_debug_name(debug_name) {}
#else

ShaderProgram::ShaderProgram() {}

#endif
/***********************************************/



ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
{
#ifdef _DEBUG
	this->m_debug_name = std::move(other.m_debug_name);
#endif
	this->m_program_id = other.m_program_id;
	other.m_program_id = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	clear();
#ifdef _DEBUG
	this->m_debug_name = std::move(other.m_debug_name);
#endif
	this->m_program_id = other.m_program_id;
	other.m_program_id = 0;

	return *this;
}

ShaderProgram::~ShaderProgram()
{
	clear();
}

GLint ShaderProgram::getLocation(const std::string& name)
{
	std::hash<std::string> hasher;
	size_t hash = hasher(name);
	GLint  location;
	auto   it = m_locations.find(hash);

	if (it == m_locations.end())
	{
		location = glGetUniformLocation(m_program_id, name.c_str());
		checkGeneralErrorGL(m_debug_name);
		if (location == -1)
		{
			LOG_ERROR_F("couldn't find uniform namely: %s", name.c_str());
			return -1;
		}
		m_locations[hash] = location;
	}
	else
	{
		location = it->second;
	}
	return location;
}

void ShaderProgram::clear()
{
	if (m_program_id)
	{
		glDeleteProgram(m_program_id);
	}
}

bool setupShader(const char* const src, GLenum shader_type, GLuint& result);
bool setupProgram(GLuint* shaders, uint count, GLuint& program);

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
	if (s_active_program == m_program_id)
	{
		return;
	}
	glUseProgram(m_program_id);
	s_active_program = m_program_id;
}

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

		std::string log;
		log.resize(log_length);
		glGetShaderInfoLog(result, log_length, nullptr, log.data());

		LOG_ERROR_F("couldn't compile shader [%s]", log.c_str());

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

	for (uint i = 0; i < count; ++i)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	GLint is_linked;
	glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
	if (is_linked == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		std::string log;
		log.resize(length);
		glGetProgramInfoLog(program, length, nullptr, log.data());
		LOG_ERROR_F("couldn't link program [%s]", log.c_str());

		glDeleteProgram(program);

		for (uint i = 0; i < count; ++i)
		{
			glDetachShader(program, shaders[i]);
			glDeleteShader(shaders[i]);
		}

		for (uint i = 0; i < count; ++i)
		{
			glDeleteShader(shaders[i]);
		}

		return false;
	}


	return true;
}
