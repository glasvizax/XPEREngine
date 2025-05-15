#pragma once

#include <limits>
#include <array>

#include "Defines.h"
#include "ShaderProgram.h"
#include "Texture.h"

/**************** COLOR ********************/
struct MP_Color
{
	void	  apply(ShaderProgram* program);
	glm::vec4 m_value = glm::vec4(1.0f);

private:
	inline static glm::vec4 s_global = glm::vec4(std::numeric_limits<float>::max());
	inline static std::string				s_name = "material.color";
};

/************ SPECULAR SCALAR ***************/
struct MP_SpecularScalar
{
	void apply(ShaderProgram* program);

	float m_value = 1.0f;

private:
	inline static float s_global = std::numeric_limits<float>::max();
	inline static std::string			s_name = "material.specular_scalar";
};

/************** SHININESS ***************/
struct MP_Shininess
{
	void apply(ShaderProgram* program);

	float m_value = 32.0f;

private:
	inline static float s_global = std::numeric_limits<float>::max();
	inline static std::string s_name = "material.shininess";
};

/************** DIFFUSE BLENDS 16 ***************/
struct MP_DiffuseBlends16
{
	void apply(ShaderProgram* program);

	std::array<float, 16> m_value{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

private:
	inline static std::array<float, 16> s_global = { std::numeric_limits<float>::max() };
	inline static std::string			s_name = "material.diffuse_blends";
};

/************** DIFFUSE BLENDS 8 ***************/
struct MP_DiffuseBlends8
{
	void apply(ShaderProgram* program);

	std::array<float, 8> m_value{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

private:
	inline static std::array<float, 8> s_global = { std::numeric_limits<float>::max() };
	inline static std::string		   s_name = "material.diffuse_blends";
};

/************** SPECULAR BLENDS 8 ***************/
struct MP_SpecularBlends8
{
	void apply(ShaderProgram* program);

	std::array<float, 8> m_value{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

private:
	inline static std::array<float, 8> s_global = { std::numeric_limits<float>::max() };
	inline static std::string		   s_name = "material.specular_blends";
};

/************* DIFFUSE 16 ***************/
struct MP_Diffuse16
{
	void apply(ShaderProgram* program);

	void pushDiffuse(Texture* texture);

	void popDiffuse();

	GLsizei getDiffuseCount();

private:
	std::array<Texture*, 16> m_value;
	GLsizei					 m_count = 0;

	inline static std::string s_count_name = "material.diffuse_count";
	inline static std::string  s_name = "material.diffuse";

	inline static bool s_first = true;
	inline static GLsizei s_global_count = std::numeric_limits<int>::max();
};

/************* DIFFUSE 8 SPECULAR 8 ***************/
struct MP_Diff8Spec8
{
	void apply(ShaderProgram* program);

	void pushDiffuse(Texture* texture);

	void popDiffuse();

	void pushSpecular(Texture* texture);

	void popSpecular();

	GLsizei getSpecularCount();

	GLsizei getDiffuseCount();

private:
	std::array<Texture*, 8> m_diff;
	std::array<Texture*, 8> m_spec;
	GLsizei					m_spec_count = 0;
	GLsizei					m_diff_count = 0;

	inline static std::string s_diff_count_name = "material.diffuse_count";
	inline static std::string s_diff_name = "material.diffuse";
	inline static std::string s_spec_count_name = "material.specular_count";
	inline static std::string s_spec_name = "material.specular";

	inline static bool	  s_first = true;
	inline static GLsizei s_global_spec_count = std::numeric_limits<int>::max();
	inline static GLsizei s_global_diff_count = std::numeric_limits<int>::max();
};

/************* DIFFUSE 7 SPECULAR 7 NORMAL ***************/
struct MP_Diff7Spec7Norm
{
	void apply(ShaderProgram* program);

	void pushDiffuse(Texture* texture);

	void popDiffuse();

	void pushSpecular(Texture* texture);

	void popSpecular();

	void setNormal(Texture* texture);

private:
	std::array<Texture*, 7> m_diff;
	std::array<Texture*, 7> m_spec;
	Texture*				m_normal = nullptr;

	GLsizei m_diff_count = 0;
	GLsizei m_spec_count = 0;

	inline static std::string s_diff_count_name = "material.diffuse_count";
	inline static std::string s_diff_name = "material.diffuse";
	inline static std::string s_spec_count_name = "material.specular_count";
	inline static std::string s_spec_name = "material.specular";

	inline static std::string s_norm_name = "material.normal";

	inline static bool	  s_first = true;
	inline static GLsizei s_global_diff_count = std::numeric_limits<int>::max();
	inline static GLsizei s_global_spec_count = std::numeric_limits<int>::max();
};

/************* DIFFUSE 7 SPECULAR 7 NORMAL HEIGHT ***************/
struct MP_Diff7Spec7NormHeight
{
	void apply(ShaderProgram* program);

	void pushDiffuse(Texture* texture);

	void pushSpecular(Texture* texture);

	void popDiffuse();

	void popSpecular();

	void setNormal(Texture* texture);

	void setHeight(Texture* texture);

private:
	std::array<Texture*, 7> m_diff;
	std::array<Texture*, 7> m_spec;
	Texture*				m_normal = nullptr;
	Texture*				m_height = nullptr;

	inline static std::string s_diff_count_name = "material.diffuse_count";
	inline static std::string s_diff_name = "material.diffuse";
	inline static std::string s_spec_count_name = "material.specular_count";
	inline static std::string s_spec_name = "material.specular";

	inline static std::string s_norm_name = "material.normal";
	inline static std::string s_height_name = "material.height";

	GLsizei m_diff_count = 0;
	GLsizei m_spec_count = 0;

	inline static bool	  s_first = true;
	inline static GLsizei s_global_diff_count = std::numeric_limits<int>::max();
	inline static GLsizei s_global_spec_count = std::numeric_limits<int>::max();
};