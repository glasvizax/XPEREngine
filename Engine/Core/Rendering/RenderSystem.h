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
#include "UniformBuffer.h"
#include "RenderStages.h"
#include "Defines.h"

class ResourceManager;

class RenderSystem
{
	friend class Engine;

	bool init();

	void render();

	void destroy();

public:
	ShaderProgramManager& getShaderProgramManager();

	Entity& getRootEntity();

	Camera& getCamera();

	void testInputH();
	void testInputK();

	void addPointLight(PointLight& point_light);

private:
	void updateMatrices();
	void initMatricesBuffer();
	void initLightsBuffer();
	void initScene();
	void initScreenQuad();

private:
	const uint MAX_POINT_LIGHTS = 100;

	ResourceManager* m_resource_manager;

	Entity m_root_entity;

	ShaderProgramManager m_shader_program_manager;

	Camera m_camera;

	UniformBuffer m_matrices_buffer;
	UniformBuffer m_lights_buffer;

	VertexArray m_screen_quad;

	GeometryStage		 m_geometry_stage;
	LightingSSAOStage	 m_ssao_stage;
	LightingAmbientStage m_lighting_ambient_stage;
	LightingFinalStage	 m_lighting_final_stage;
	PostProcessStage	 m_postprocess_stage;
};
