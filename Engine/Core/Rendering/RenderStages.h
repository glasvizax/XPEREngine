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
	void init(
		ShaderProgram* ssao_base_sp,
		ShaderProgram* ssao_blur_sp,
		GeometryStage* geometry_stage,
		VertexArray*   screen_quad,
		int			   power = 8,
		float		   radius_factor = 0.15f,
		int			   blur_iterations = 4);

	void run();

	void setPower(int power);
	void setRadiusFactor(float radius_factor);
	void setBlurIterations(int blur_iterations);

	ShaderProgram* m_ssao_base_sp;
	ShaderProgram* m_ssao_blur_sp;

	Framebuffer m_ssao_base_fb;
	Framebuffer m_ssao_blur_fb;

	Texture m_output_ssao_blur_tex;
	Texture m_ssao_noise_tex;
	Texture m_ssao_base_tex;

	VertexArray* m_screen_quad;

	float m_radius_factor = 0.15f;
	int	  m_power = 8;
	int	  m_blur_iterations = 4;

	const int KERNEL_SIZE = 64;
	const int NOISE_SIZE = 16;
};

class LightingAmbientStage
{
public:
	void init(
		ShaderProgram*	   ambient_sp,
		GeometryStage*	   geometry_stage,
		LightingSSAOStage* ssao_stage,
		VertexArray*	   screen_quad,
		float			   ambient_factor = 1.0f);

	void run();

	void setAmbientFactor(float ambient_factor);

	VertexArray*   m_screen_quad;
	Framebuffer	   m_lighting_ambient_fb;
	Texture		   m_output_ambient_tex;
	ShaderProgram* m_ambient_sp;

	float m_ambient_factor;
};

class LightingShadowMappingStage
{
public:
	void init(
		ShaderProgram* depthmap_sp,
		GeometryStage* geometry_stage,
		int depthmap_size, float depthmap_near,
		float					 depthmap_far,
		std::vector<PointLight>* point_lights);

	void run();

	ShaderProgram*			 m_depthmap_sp;
	std::vector<PointLight>* m_point_lights;
	Framebuffer				 m_depthmap_fb;
	Entity*					 m_root_entity;
	std::vector<Cubemap>	 m_output_depthmaps;
	xm::mat4				 m_light_projection;

	int m_main_width;
	int m_main_height;
	int m_depthmap_size;

	float m_depthmap_far;
	float m_depthmap_near;
};

class LightingFinalStage
{
public:
	void init(
		ShaderProgram*				diffspec_sp,
		LightingAmbientStage*		ambient_stage,
		LightingShadowMappingStage* shadow_mapping_stage,
		std::vector<PointLight>*	point_lights,
		Camera*						camera);

	void run();

	Framebuffer* m_lighting_final_fb;
	Texture*	 m_output_lighting_tex;

	std::vector<PointLight>* m_point_lights;
	std::vector<Cubemap>*	 m_input_depthmaps;

	Camera*		   m_camera;
	ShaderProgram* m_diffspec_sp;
	Mesh		   m_light_volume;
};

class ForwardStage
{
public:
	void init(
		ShaderProgram*		forward_color_sp,
		GeometryStage*		geometry_stage,
		LightingFinalStage* lighting_final_stage);

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
	void init(ShaderProgram* skybox_sp, ForwardStage* forward_stage, GeometryStage* geometry_stage, Cubemap* skybox_cm, Camera* camera);

	void run();

	void setSkyboxCubemap(Cubemap* skybox_cm);

	Renderbuffer* m_input_rb;
	Texture*	  m_output_tex;

	Framebuffer	   m_skybox_fb;
	ShaderProgram* m_skybox_sp;
	Cubemap*	   m_skybox_cm;
	VertexArray	   m_skybox_va;
	Camera*		   m_camera;
};

class BloomStage
{
public:
	void init(ShaderProgram* brightness_extraction, ShaderProgram* blur, SkyboxStage* skybox_stage, VertexArray* screen_quad, float threshold = 1.2f);

	void run();

	void setThreshold(float threshold);

	VertexArray*   m_screen_quad;
	ShaderProgram* m_brightness_extraction_sp;
	ShaderProgram* m_blur_sp;
	Texture*	   m_input_tex;

	Texture		m_brightness_extraction_tex;
	Framebuffer m_brightness_extraction_fb;

	Framebuffer m_blur_pingpong_fbs[2];
	Texture		m_blur_pingpong_texs[2];

	float m_threshold;
};

class PostProcessStage
{
public:
	void init(ShaderProgram* postprocess_sp, VertexArray* screen_quad, float exposure = 1.0f);

	void run();

	void setExposure(float new_exposure);

	VertexArray*   m_screen_quad;
	ShaderProgram* m_postprocess_shader;

	float m_exposure;
};