#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>&& vertices)
{
	m_vertices = std::move(vertices);
	m_vertices_count = m_vertices.size();
	m_indices_count = 0;
	m_draw_element = false;

	setupVertexArray();
	clearRAM();
}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<uint>&& indices)
{
	m_vertices = std::move(vertices);
	m_indices = std::move(indices);
	m_vertices_count = m_vertices.size();
	m_indices_count = m_indices.size();
	m_draw_element = true;

	setupVertexArray();
	setupElementsArray();
	clearRAM();
}

Mesh::Mesh(const std::vector<Vertex>& vertices)
{
	m_vertices = vertices;
	m_vertices_count = m_vertices.size();
	m_indices_count = 0;
	m_draw_element = false;

	setupVertexArray();
	clearRAM();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint>& indices)
{
	m_vertices = vertices;
	m_indices = indices;
	m_vertices_count = m_vertices.size();
	m_indices_count = m_indices.size();
	m_draw_element = true;

	setupVertexArray();
	setupElementsArray();
	clearRAM();
}

void Mesh::draw()
{
	vertex_array.bind();
	if (m_draw_element)
	{
		glDrawElements(GL_TRIANGLES, m_indices_count, GL_UNSIGNED_INT, nullptr);
		
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_vertices_count);
	}
	checkGeneralErrorGL("mesh");
}

void Mesh::setupVertexArray()
{
	vertex_array.init();
	vertex_array.bind();
	checkGeneralErrorGL("mesh");
	vertex_array.attachArrayBuffer(m_vertices.size() * sizeof(Vertex), m_vertices.data());
	vertex_array.autoEnableAttributes<Vertex>();
	checkGeneralErrorGL("mesh");
}

void Mesh::setupElementsArray()
{
	vertex_array.attachElementBuffer(m_indices.size() * sizeof(uint), m_indices.data());
	checkGeneralErrorGL("mesh");
}

void Mesh::clearRAM()
{
	m_vertices.clear();
	m_indices.clear();
}

Mesh generateIdenticalCube()
{
	std::vector<Vertex> vertices = {
		// Back face
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) }, // bottom-left
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },	 // top-right
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },	 // bottom-right
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },	 // top-right
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) }, // bottom-left
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },	 // top-left

		// Front face
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) }, // bottom-left
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },  // bottom-right
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },   // top-right
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },   // top-right
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },  // top-left
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) }, // bottom-left

		// Left face
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },	 // top-right
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },	 // top-left
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // bottom-left
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // bottom-left
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },	 // bottom-right
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },	 // top-right

		// Right face
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },   // top-left
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // bottom-right
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },  // top-right
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // bottom-right
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },   // top-left
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },  // bottom-left

		// Bottom face
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // top-right
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },	 // top-left
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },	 // bottom-left
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },	 // bottom-left
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },	 // bottom-right
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // top-right

		// Top face
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // top-left
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },   // bottom-right
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },  // top-right
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },   // bottom-right
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) }, // top-left
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }   // bottom-left
	};

	Mesh cube(std::move(vertices));
	return cube;
}

Mesh generateSphere(float radius, uint sector_count, uint stack_count)
{
	size_t				vertex_amount = (sector_count + 1) * (stack_count + 1);
	std::vector<Vertex> vertices;
	vertices.reserve(vertex_amount);

	float sector_step = 2 * glm::pi<float>() / sector_count;
	float stack_step = glm::pi<float>() / stack_count;
	float sector_angle, stack_angle;
	float length_inv = 1.0f / radius;

	for (uint i = 0; i <= stack_count; ++i)
	{
		stack_angle = glm::pi<float>() / 2 - i * stack_step;
		float xz = radius * cosf(stack_angle);
		float y = radius * sinf(stack_angle);

		for (uint j = 0; j <= sector_count; ++j)
		{
			sector_angle = j * sector_step;

			Vertex v;
			// vertex position (x, y, z)
			float x = xz * cosf(sector_angle);
			float z = -xz * sinf(sector_angle);

			v.m_position = glm::vec3(x, y, z);

			// normalized vertex normal (nx, ny, nz)
			float nx = x * length_inv;
			float ny = y * length_inv;
			float nz = z * length_inv;

			v.m_normal = glm::vec3(nx, ny, nz);

			// vertex tex coord (s, t) range between [0, 1]
			float s = (float)j / sector_count;
			float t = (float)i / stack_count;

			v.m_uv = glm::vec2(s, t);

			vertices.push_back(v);
		}
	}
	std::vector<uint> indices;
	indices.reserve(stack_count * sector_count * 6);
	int k1, k2;
	for (uint i = 0; i < stack_count; ++i)
	{
		k1 = i * (sector_count + 1);
		k2 = k1 + sector_count + 1;

		for (uint j = 0; j < sector_count; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stack_count - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	Mesh sphere(std::move(vertices), std::move(indices));
	return sphere;
}
