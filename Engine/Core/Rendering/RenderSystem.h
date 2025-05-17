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
#include "Model.h"

class RenderSystem
{
	friend class Engine;

	bool init();

	void render();

	void destroy();

	Entity m_root_entity;

	ShaderProgramManager m_shader_program_manager; 

	//Mesh m_cube_mesh;

	uint m_matrices_ubo;

	Camera m_camera;

	//Model backpack;
	//Texture wood_tex;

public:

	ShaderProgramManager& getShaderProgramManager() 
	{
		return m_shader_program_manager;
	}

	Entity& getRootEntity() 
	{
		return m_root_entity;
	}

	Camera& getCamera();

	void testInputH();
	void testInputK();
};
