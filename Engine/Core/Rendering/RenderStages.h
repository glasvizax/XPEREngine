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
#include "Cubemap.h"

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

	Framebuffer	 m_geometry_fb;
	Renderbuffer m_geometry_rb;
	Entity*		 m_root_entity;
};

class LightingSSAOStage
{
public:
	void init(ShaderProgram* ssao_base_sp, ShaderProgram* ssao_blur_sp, int width, int height, VertexArray* screen_quad);

	void run();

	ShaderProgram* m_ssao_base_sp;
	ShaderProgram* m_ssao_blur_sp;

	Framebuffer m_ssao_base_fb;
	Framebuffer m_ssao_blur_fb;

	Texture m_output_ssao_blur_tex;
	Texture m_ssao_noise_tex;
	Texture m_ssao_base_tex;

	VertexArray* m_screen_quad;

	const int KERNEL_SIZE = 64;
	const int NOISE_SIZE = 16;
};

class LightingAmbientStage
{
public:
	void init(ShaderProgram* ambient_sp, GeometryStage* geometry_stage, LightingSSAOStage* ssao_stage, int width, int height, VertexArray* screen_quad);

	void run();

	VertexArray*   m_screen_quad;
	Framebuffer	   m_lighting_ambient_fb;
	Texture		   m_output_ambient_tex;
	ShaderProgram* m_ambient_sp;
};

class LightingShadowMappingStage
{
public:
	void init(ShaderProgram* depthmap_sp, int width, int height, int depthmap_size, float depthmap_near, float depthmap_far, Entity* root_entity, std::vector<PointLight>* point_lights);

	void run();

	ShaderProgram*			 m_depthmap_sp;
	std::vector<PointLight>* m_point_lights;
	Framebuffer				 m_depthmap_fb;
	Entity*					 m_root_entity;
	std::vector<Cubemap>	 m_output_depthmaps;
	glm::mat4				 m_light_projection;

	int m_main_width;
	int m_main_height;
	int m_depthmap_size;

	float m_depthmap_far;
	float m_depthmap_near;
};

class LightingFinalStage
{
public:
	void init(ShaderProgram* diffspec_sp, LightingAmbientStage* ambient_stage, LightingShadowMappingStage* shadow_mapping_stage, std::vector<PointLight>* point_lights, Camera* camera);

	void run();

	Framebuffer* m_lighting_final_fb;
	Texture*	 m_output_lighting_tex;

	std::vector<PointLight>* m_point_lights;
	std::vector<Cubemap>*	 m_input_depthmaps;

	// std::optional<DirLight> m_dir_light = std::nullopt;

	Camera*		   m_camera;
	ShaderProgram* m_diffspec_sp;
	Mesh		   m_light_volume;
};

class ForwardStage
{
public:
	void init(ShaderProgram* forward_color_sp, GeometryStage* geometry_stage, LightingFinalStage* lighting_final_stage);

	void run();

	Renderbuffer*			 m_geomentry_rb;
	Texture*				 m_output_tex;
	std::vector<PointLight>* m_point_lights;

	Framebuffer	   m_forward_fb;
	Mesh		   m_light_sphere;
	ShaderProgram* m_forward_color_sp;
};


class SkyboxStage
{
public:
	void init(ShaderProgram* skybox_sp, ForwardStage* forward_stage, GeometryStage* geometry_stage, Cubemap&& skybox_cm, Camera* camera);

	void run();

	Renderbuffer*  m_input_rb;
	Texture*	   m_output_tex;

	Framebuffer	   m_skybox_fb;
	ShaderProgram* m_skybox_sp;
	Cubemap		   m_skybox_cm;
	VertexArray	   m_skybox_va;
	Camera*		   m_camera;

};

class PostProcessStage
{
public:
	void init(ShaderProgram* postprocess_sp, VertexArray* screen_quad);

	void run();

	VertexArray*   m_screen_quad;
	ShaderProgram* m_postprocess_shader;
};