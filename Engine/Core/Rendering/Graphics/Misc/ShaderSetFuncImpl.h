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
		std::is_same_v<T, float> || 
		std::is_same_v<T, double> || 
		std::is_same_v<T, int> || 
		std::is_same_v<T, uint> ||
		std::is_same_v<T, size_t> || 
		std::is_same_v<T, bool>,
		"T must be float, double, int or uint"
	);

	use();
	checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(name);
	checkGeneralErrorGL(m_debug_name);
	uniformVecFunc<N, T>(location, vec);
	checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void ShaderProgram::setVecArray(const std::string& name, const glm::vec<N, T>* const array, GLsizei count)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");
	static_assert(
		std::is_same_v<T, float> || 
		std::is_same_v<T, double> || 
		std::is_same_v<T, int> || 
		std::is_same_v<T, uint> || 
		std::is_same_v<T, size_t> || 
		std::is_same_v<T, bool>,
		"T must be float, double, int or uint"
	);

	use();
	checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(name);
	uniformVecArrayFunc<N, T>(location, array, count);
	checkGeneralErrorGL(m_debug_name);
}

template <typename T>
inline void ShaderProgram::set(const std::string& name, T val)
{
	static_assert(
		std::is_same_v<T, float> || 
		std::is_same_v<T, double> || 
		std::is_same_v<T, int> || 
		std::is_same_v<T, uint> || 
		std::is_same_v<T, size_t> || 
		std::is_same_v<T, bool>,
		"T must be float, double, int uint or size_t"
	);

	use();
	checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(name);
	uniformValueFunc<T>(location, val);
	checkGeneralErrorGL(m_debug_name);
}

template <typename T>
inline void ShaderProgram::setArray(const std::string& name, const T* const vals, GLsizei count)
{
	static_assert(
		std::is_same_v<T, float> || 
		std::is_same_v<T, double> || 
		std::is_same_v<T, int> || 
		std::is_same_v<T, uint> || 
		std::is_same_v<T, size_t> || 
		std::is_same_v<T, bool>,
		"T must be float, double, int or uint"
	);

	use();
	checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation();
	uniformValueArrayFunc<T>(location, vals, count);
	checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void ShaderProgram::setMat(const std::string& name, const glm::mat<N, N, T>& mat)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");

	static_assert(
		std::is_same_v<T, float> || 
		std::is_same_v<T, double>,
		"T must be float, double"
	);

	use();
	checkGeneralErrorGL(m_debug_name);
	GLint location = getLocation(name);
	uniformMatArrayFunc<N, T>(location, &mat, 1);
	checkGeneralErrorGL(m_debug_name);
}

template <size_t N, typename T>
inline void ShaderProgram::setMatArray(const std::string& name, const glm::mat<N, N, T>* const array, GLsizei count)
{
	static_assert(N <= 4 && N >= 2, "N must be between 2 and 4");

	static_assert(
		std::is_same_v<T, float> || 
		std::is_same_v<T, double>,
		"T must be float, double, int or uint"
	);

	use();
	checkGeneralErrorGL(m_debug_name);

	GLint location = getLocation(name);
	checkGeneralErrorGL(m_debug_name);
	uniformMatArrayFunc<N, T>(location, array, count);
	checkGeneralErrorGL(m_debug_name);
}
