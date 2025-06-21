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
#include "xm/misc_helpers.h"

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

	uint addPointLight(PointLight& point_light);
	void deletePointLight(uint light_index);

	void	  setPointLightPosition(uint light_index, glm::vec3 new_position);
	glm::vec3 getPointLightPosition(uint light_index);

	void	  setPointLightDiffuse(uint light_index, glm::vec3 new_diffuse);
	glm::vec3 getPointLightDiffuse(uint light_index);

	void	  setPointLightSpecular(uint light_index, glm::vec3 new_specular);
	glm::vec3 getPointLightSpecular(uint light_index);

	void	  setPointLightAmbient(uint light_index, glm::vec3 new_ambient);
	glm::vec3 getPointLightAmbient(uint light_index);

	void  setPointLightLinear(uint light_index, float new_linear);
	float getPointLightLinear(uint light_index);

	void  setPointLightQuadratic(uint light_index, float new_quadratic);
	float getPointLightQuadratic(uint light_index);

private:
	void updateMatrices();
	void initMatricesBuffer();
	void initLightsBuffer();
	void initScene();
	void initScreenQuad();

private:
	const uint MAX_POINT_LIGHTS = 32;

	ResourceManager* m_resource_manager;

	Entity m_root_entity;

	ShaderProgramManager m_shader_program_manager;
	Camera				 m_camera;

	Cubemap m_daylight;
	Cubemap m_night;

	UniformBuffer m_matrices_buffer;
	UniformBuffer m_lights_buffer;

	VertexArray m_screen_quad;

	std::vector<PointLight> m_point_lights;

	GeometryStage			   m_geometry_stage;
	LightingSSAOStage		   m_ssao_stage;
	LightingAmbientStage	   m_ambient_stage;
	LightingShadowMappingStage m_shadow_mapping_stage;
	LightingFinalStage		   m_lighting_final_stage;
	ForwardStage			   m_forward_stage;
	SkyboxStage				   m_skybox_stage;
	BloomStage				   m_bloom_stage;
	PostProcessStage		   m_postprocess_stage;
};
