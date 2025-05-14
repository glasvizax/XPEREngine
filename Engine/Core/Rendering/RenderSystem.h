#pragma once 

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Material.h"
#include "Scene.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class RenderSystem
{
	friend class Engine;

	bool init();

	void render();

	void destroy();

	Entity m_scene_root;

	ShaderProgram m_color_program;
	MaterialC	  m_color_material;

	Mesh m_cube_mesh;

	uint m_matrices_ubo;

	Camera m_camera;

public:

	Camera& getCamera();
};
