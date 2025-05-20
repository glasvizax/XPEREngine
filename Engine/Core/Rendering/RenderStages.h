#pragma once
#include <glad/glad.h>
#include <vector>
#include <optional>

#include <glm/glm.hpp>

#include "Framebuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Mesh.h"
#include "Light.h"

class ShaderProgram;
class Entity;
class ResourceManager;
class Camera;

class GeometryStage
{
public:
	void init(int width, int height, Entity* root_entity);

	void run();

	Texture m_output_position_tex;
	Texture m_output_normal_shininess_tex;
	Texture m_output_diffuse_specular_tex;

	Framebuffer m_geometry_fb;

private:
	Entity* m_root_entity;
};

class LightingSSAOStage
{
public:
	void init(int width, int height, VertexArray* screen_quad, ShaderProgram* ssao_base_sp, ShaderProgram* ssao_blur_sp, GeometryStage* geometry_stage, int kernel_size = 64, int noise_size = 16);

	void run();

	ShaderProgram* m_ssao_base_sp;
	ShaderProgram* m_ssao_blur_sp;

	GeometryStage* m_geometry_stage;

	Framebuffer m_ssao_base_fb;
	Framebuffer m_ssao_blur_fb;

	Texture m_output_ssao_blur_tex;
	Texture m_ssao_noise_tex;
	Texture m_ssao_base_tex;

	VertexArray* m_screen_quad;
};

class LightingAmbientStage
{
public:
	void init(int width, int height, VertexArray* screen_quad, ShaderProgram* ambient_sp, GeometryStage* geometry_stage, LightingSSAOStage* ssao_stage);

	void run();

	ShaderProgram* m_ambient_sp;

	GeometryStage* m_geometry_stage;

	LightingSSAOStage* m_ssao_stage;

	Framebuffer m_lighting_fb;

	Texture m_output_lighting_tex;

	VertexArray* m_screen_quad;
};

class LightingFinalStage
{
public:
	void init(Camera* camera, ShaderProgram* diffspec_sp, GeometryStage* geometry_stage, LightingAmbientStage* ambient_stage);

	void run();

	Texture* m_output_lighting;

	std::vector<PointLight> m_point_lights;
	// std::optional<DirLight> m_dir_light = std::nullopt;

	Camera*				  m_camera;
	GeometryStage*		  m_geometry_stage;
	LightingAmbientStage* m_ambient_stage;
	ShaderProgram*		  m_diffspec_sp;
	Mesh				  m_light_volume;
};

class PostProcessStage
{
public:
	void init(ShaderProgram* postprocess_sp, VertexArray* screen_quad);

	void run();

	VertexArray*   m_screen_quad;
	ShaderProgram* m_postprocess_shader;
};