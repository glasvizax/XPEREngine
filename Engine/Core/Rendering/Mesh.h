#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "Texture.h"
#include "ShaderProgram.h"
#include "Debug.h"
#include "Defines.h"
#include "VertexArray.h"
#include "Material.h"

struct Vertex
{
	xm::vec3 m_position;
	xm::vec3 m_normal;
	xm::vec2 m_uv;
};

struct VertexTB
{
	xm::vec3 m_position;
	xm::vec3 m_normal;
	xm::vec2 m_uv;
	xm::vec3 m_tangent;
	xm::vec3 m_bitangent;
};

struct Mesh
{
	friend class RenderSystem;

	Mesh() = default;
	~Mesh();

public:
	template <typename VertexType>
	Mesh(const std::vector<VertexType>& vertices);

	template <typename VertexType>
	Mesh(const std::vector<VertexType>& vertices, const std::vector<uint>& indices);

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void draw();

	void clear();

private:
	GLsizei m_vertices_count = 0;
	GLsizei m_indices_count = 0;

	bool m_draw_element = true;

	VertexArray vertex_array; // TODO : improve allocation
};

Mesh generateIdenticalCube();
Mesh generateSphere(float radius, uint sector_count, uint stack_count);

template <>
inline void VertexArray::autoEnableAttributes<Vertex>()
{
	enableAttribute(0, 3, 8, 0);
	enableAttribute(1, 3, 8, 3);
	enableAttribute(2, 2, 8, 6);
}

template <>
inline void VertexArray::autoEnableAttributes<VertexTB>()
{
	enableAttribute(0, 3, 14, 0);
	enableAttribute(1, 3, 14, 3);
	enableAttribute(2, 2, 14, 6);
	enableAttribute(3, 3, 14, 8);
	enableAttribute(4, 3, 14, 11);
}

template <typename VertexType>
inline Mesh::Mesh(const std::vector<VertexType>& vertices)
{
	m_vertices_count = vertices.size();
	m_draw_element = false;

	vertex_array.init();
	vertex_array.bind();
	checkGeneralErrorGL("mesh");
	vertex_array.attachArrayBuffer(vertices.size() * sizeof(VertexType), vertices.data());
	vertex_array.autoEnableAttributes<VertexType>();
	checkGeneralErrorGL("mesh");
}

template <typename VertexType>
inline Mesh::Mesh(const std::vector<VertexType>& vertices, const std::vector<uint>& indices)
{
	m_vertices_count = vertices.size();
	m_indices_count = indices.size();
	m_draw_element = true;

	vertex_array.init();
	vertex_array.bind();
	checkGeneralErrorGL("mesh");
	vertex_array.attachArrayBuffer(vertices.size() * sizeof(VertexType), vertices.data());
	vertex_array.autoEnableAttributes<VertexType>();
	checkGeneralErrorGL("mesh");

	vertex_array.attachElementBuffer(indices.size() * sizeof(uint), indices.data());
	checkGeneralErrorGL("mesh");
}
