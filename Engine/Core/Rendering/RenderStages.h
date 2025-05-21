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

	Framebuffer	 m_geometry_fb;
	Renderbuffer m_geometry_rb;
	Entity*		 m_root_entity;
};

class LightingSSAOStage
{
public:
	void init(int width, int height, VertexArray* screen_quad, ShaderProgram* ssao_base_sp, ShaderProgram* ssao_blur_sp);

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
	void init(int width, int height, VertexArray* screen_quad, ShaderProgram* ambient_sp, GeometryStage* geometry_stage, LightingSSAOStage* ssao_stage);

	void run();

	VertexArray*   m_screen_quad;
	Framebuffer	   m_lighting_fb;
	Texture		   m_output_ambient_tex;
	ShaderProgram* m_ambient_sp;
};

class LightingFinalStage
{
public:
	void init(Camera* camera, ShaderProgram* diffspec_sp, LightingAmbientStage* ambient_stage);

	void run();

	Texture* m_output_lighting_tex;

	std::vector<PointLight> m_point_lights;
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

	Renderbuffer* m_geomentry_rb;
	Texture*	  m_input_lighting_tex;
	std::vector<PointLight>* m_point_lights;

	Framebuffer	   m_forward_fb;
	Mesh		   m_light_sphere;
	ShaderProgram* m_forward_color_sp;
};

class PostProcessStage
{
public:
	void init(ShaderProgram* postprocess_sp, VertexArray* screen_quad);

	void run();

	VertexArray*   m_screen_quad;
	ShaderProgram* m_postprocess_shader;
};