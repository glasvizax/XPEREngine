#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Material.h"
#include "Entity.h"
#include "Camera.h"
#include "ShaderProgramManager.h"

class RenderSystem
{
	friend class Engine;

	bool init();

	void render();

	void destroy();

	Entity m_scene_root;

	ShaderProgramManager m_shader_program_manager; 

	Mesh m_cube_mesh;

	uint m_matrices_ubo;

	Camera m_camera;

	Texture wood_tex;

public:

	Camera& getCamera();

	void testInputH();
	void testInputK();
};
