#pragma once

#include <string>
#include <unordered_map>
#include <type_traits>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

#include "Debug.h"
#include "Defines.h"
#include "DebugOpenGL.h"

class ShaderProgram
{

public:
	ShaderProgram();

#ifdef _DEBUG
	std::string m_debug_name;
	ShaderProgram(const std::string& debug_name);
#endif

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

	~ShaderProgram();

	bool init(const char* const vertex_src, const char* const fragment_src);
	bool init(const char* const vertex_src, const char* const fragment_src, const char* const geometry_src);

	void use() const;

	template <size_t N, typename T>
	void setVec(const std::string& name, glm::vec<N, T> vec);

	template <size_t N, typename T>
	void setVecArray(const std::string& name, const glm::vec<N, T>* const array, GLsizei count);

	template <typename T>
	void set(const std::string& name, T val);

	template <typename T>
	void setArray(const std::string& name, const T* const vals, GLsizei count);

	template <size_t N, typename T>
	void setMat(const std::string& name, const glm::mat<N, N, T>& mat);

	template <size_t N, typename T>
	void setMatArray(const std::string& name, const glm::mat<N, N, T>* const array, GLsizei count);

	GLuint getID() const { return m_program_id; }

private:
	GLint getLocation(const std::string& name);

	void clear();

	GLuint							  m_program_id = 0;
	std::unordered_map<size_t, GLint> m_locations;

	inline static GLuint s_active_program = 0;
};

#include "ShaderSetFuncImpl.h"