#pragma once

namespace
{
	#include "ShaderUniformFuncHelper.h"
} // namespace

template <size_t N, typename T>
inline void ShaderProgram::setVec(const std::string& name, glm::vec<N, T> vec)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, int> || std::is_same_v<T, uint>,
		"T must be float, double, int or uint");

	use();
	checkGeneralErrorGL(m_debug_name);

	size_t			 hash = m_hasher(name);
	GLint			 location;
	auto			 it = m_locations.find(hash);

	if (it == m_locations.end())
	{
		location = glGetUniformLocation(m_program_id, name.c_str());
		checkGeneralErrorGL(m_debug_name);
		if (location == -1)
		{
			LOG_ERROR_F("couldn't find uniform namely: %s", name.c_str());
			return;
		}
		m_locations[hash] = location;
	}
	else
	{
		location = it->second;
	}

	checkGeneralErrorGL(m_debug_name);
	uniformVecFunc<N, T>(location, vec);
	checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void ShaderProgram::setVecArray(const std::string& name, const std::vector<glm::vec<N, T>>& array, GLsizei count)
{

	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, int> || std::is_same_v<T, uint>,
		"T must be float, double, int or uint");

	use();
	checkGeneralErrorGL(m_debug_name);
	if (array.size() != count)
	{
		if (count > array.size())
		{
			LOG_ERROR_S("count > array.size()");
		}
		else
		{
			LOG_WARNING_S("count < array.size()");
		}
	}

	size_t			 hash = m_hasher(name);
	GLint			 location;
	auto			 it = m_locations.find(hash);
	if (it == m_locations.end())
	{
		location = glGetUniformLocation(m_program_id, name.c_str());
		checkGeneralErrorGL(m_debug_name);
		if (location == -1)
		{
			LOG_ERROR_F("couldn't find uniform namely: %s", name.c_str());
			return;
		}
		m_locations[hash] = location;
	}
	else
	{
		location = it->second;
	}
	uniformVecArrayFunc<N, T>(location, array.data());
	checkGeneralErrorGL(m_debug_name);
}

template <typename T>
inline void ShaderProgram::set(const std::string& name, T val)
{
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, int> || std::is_same_v<T, bool> || std::is_same_v<T, uint> || std::is_same_v <T, size_t>,
		"T must be float, double, int or uint");

	use();
	checkGeneralErrorGL(m_debug_name);

	std::string_view sv(name);
	size_t			 hash = m_hasher(name);
	GLint			 location;
	auto			 it = m_locations.find(hash);
	if (it == m_locations.end())
	{
		location = glGetUniformLocation(m_program_id, name.c_str());
		checkGeneralErrorGL(m_debug_name);
		if (location == -1)
		{
			LOG_ERROR_F("couldn't find uniform namely: %s", name.c_str());
			return;
		}
		m_locations[hash] = location;
	}
	else
	{
		location = it->second;
	}
	uniformValueFunc<T>(location, val);
	checkGeneralErrorGL(m_debug_name);
}

template <typename T>
inline void ShaderProgram::setArray(const std::string& name, T* vals, GLsizei count)
{
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, int> || std::is_same_v<T, bool> || std::is_same_v<T, uint>,
		"T must be float, double, int or uint");

	use();
	checkGeneralErrorGL(m_debug_name);
	std::string_view sv(name);
	size_t			 hash = m_hasher(name);
	GLint			 location;
	auto			 it = m_locations.find(hash);
	if (it == m_locations.end())
	{
		location = glGetUniformLocation(m_program_id, name.c_str());
		checkGeneralErrorGL(m_debug_name);
		if (location == -1)
		{
			LOG_ERROR_F("couldn't find uniform namely: %s", name.c_str());
			return;
		}
		m_locations[hash] = location;
	}
	else
	{
		location = it->second;
	}
	uniformValueArrayFunc<T>(location, vals, count);
	checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void ShaderProgram::setMat(const std::string& name, const glm::mat<N, N, T>& mat)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");
	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double>,
		"T must be float, double");

	use();
	checkGeneralErrorGL(m_debug_name);
	size_t			 hash = m_hasher(name);
	GLint			 location;
	auto			 it = m_locations.find(hash);
	if (it == m_locations.end())
	{
		location = glGetUniformLocation(m_program_id, name.c_str());
		checkGeneralErrorGL(m_debug_name);
		if (location == -1)
		{
			LOG_ERROR_F("couldn't find uniform namely: %s", name.c_str());
			return;
		}
		m_locations[hash] = location;
	}
	else
	{
		location = it->second;
	}

	uniformMatArrayFunc<N, T>(location, &mat, 1);
	checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void ShaderProgram::setMatArray(const std::string& name, const std::vector<glm::mat<N, N, T>>& array, GLsizei count)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");

	static_assert(
		std::is_same_v<T, float> || std::is_same_v<T, double>,
		"T must be float, double, int or uint");

	use();
	checkGeneralErrorGL(m_debug_name);
	if (array.size() != count)
	{
		if (count > array.size())
		{
			LOG_ERROR_S("count > array.size()");
		}
		else
		{
			LOG_WARNING_S("count < array.size()");
		}
	}

	size_t			 hash = m_hasher(name);
	GLint			 location;
	auto			 it = m_locations.find(hash);
	if (it == m_locations.end())
	{
		location = glGetUniformLocation(m_program_id, name.c_str());
		checkGeneralErrorGL(m_debug_name);
		if (location == -1)
		{
			LOG_ERROR_F("couldn't find uniform namely: %s", name.c_str());
			return;
		}
		m_locations[hash] = location;
	}
	else
	{
		location = it->second;
	}
	uniformMatArrayFunc<N, T>(location, array.data(), count);
	checkGeneralErrorGL(m_debug_name);
}
