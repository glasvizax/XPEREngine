#pragma once

#include <string>
#include <unordered_map>
#include <type_traits>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "Debug.h"
#include "Defines.h"

class Shader
{

public:
	Shader() = default;
	Shader(const char* const vertex_src, const char* const fragment_src);
	Shader(const char* const vertex_src, const char* const fragment_src, const char* const geometry_src);

	template <size_t N, typename T>
	void setVec(const std::string& name, glm::vec<N, T> vec);

	template <size_t N, typename T>
	void setVecArray(const std::string& name, const std::vector<glm::vec<N, T>>& array, GLsizei count);

	template <typename T>
	void set(const std::string& name, T val);

	template <size_t N, typename T>
	void setMat(const std::string& name, const glm::mat<N,N,T>& mat);

	template <size_t N, typename T>
	void setMatArray(const std::string& name, const std::vector<glm::mat<N, N, T>>& array, GLsizei count);

	GLuint getID() { return m_program_id; }

private:
	GLuint							  m_program_id = 0;
	std::hash<std::string_view>		  m_hasher;
	std::unordered_map<size_t, GLint> m_locations;
};

#include "ShaderSetFuncImpl.h"