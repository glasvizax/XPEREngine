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
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_uv;
};

struct Mesh
{
	friend class RenderSystem;

	Mesh() = default;

public:
	Mesh(std::vector<Vertex>&& vertices);

	Mesh(std::vector<Vertex>&& vertices, std::vector<uint>&& indices);

	Mesh(const std::vector<Vertex>& vertices);

	Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices);

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(Mesh&&) noexcept = default;

	void draw();

private:
	std::vector<Vertex> m_vertices;
	std::vector<uint>	m_indices;

	GLsizei m_vertices_count;
	GLsizei m_indices_count;

	bool m_draw_element = true;

	VertexArray vertex_array; // TODO : improve allocation

	void setupVertexArray();
	void setupElementsArray();

	void clearRAM();
};

Mesh generateIdenticalCube();
Mesh generateSphere(float radius, uint sector_count, uint stack_count);

template<typename MaterialClass>
struct ModelEntry
{
	Mesh* mesh;
	MaterialClass material;
};

struct Model
{
	std::vector<ModelEntry<MaterialC>> color_entries;
	std::vector<ModelEntry<MaterialD>> diffuse_entries;
	std::vector<ModelEntry<MaterialDS>> diffspec_entries;
};

template <>
inline void VertexArray::autoEnableAttributes<Vertex>()
{
	enableAttribute(0, 3, 8, 0);
	enableAttribute(1, 3, 8, 3);
	enableAttribute(2, 2, 8, 6);
}