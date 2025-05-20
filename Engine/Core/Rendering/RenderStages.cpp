#include "RenderStages.h"

#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "ResourceManager.h"
#include "Entity.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"

const GLuint POSITION_UNIT = 7;
const GLuint NORMAL_SHININESS_UNIT = 8;
const GLuint DIFFUSE_SPECULAR_UNIT = 9;
const GLuint SSAO_NOISE_UNIT = 10;
const GLuint SSAO_BASE_UNIT = 11;
const GLuint SSAO_BLUR_UNIT = 12;
const GLuint POSTPROCESS_INPUT_UNIT = 13;

const glm::mat4 IDENTICAL_MATRIX = glm::mat4(1.0f);

void GeometryStage::init(int width, int height, Entity* root_entity)
{
	m_root_entity = root_entity;
	m_output_position_tex.init(width, height, GL_RGBA16F, 4, false);
	m_output_position_tex.setMinFilter(GL_NEAREST);
	m_output_position_tex.setMagFilter(GL_NEAREST);
	m_output_position_tex.setWrapS(GL_CLAMP_TO_EDGE);
	m_output_position_tex.setWrapT(GL_CLAMP_TO_EDGE);

	m_output_normal_shininess_tex.init(width, height, GL_RGBA16F, 4, false);
	m_output_normal_shininess_tex.setMinFilter(GL_NEAREST);
	m_output_normal_shininess_tex.setMagFilter(GL_NEAREST);
	m_output_normal_shininess_tex.setWrapS(GL_CLAMP_TO_EDGE);
	m_output_normal_shininess_tex.setWrapT(GL_CLAMP_TO_EDGE);

	m_output_diffuse_specular_tex.init(width, height, GL_RGBA16F, 4, false);
	m_output_diffuse_specular_tex.setMinFilter(GL_NEAREST);
	m_output_diffuse_specular_tex.setMagFilter(GL_NEAREST);
	m_output_diffuse_specular_tex.setWrapS(GL_CLAMP_TO_EDGE);
	m_output_diffuse_specular_tex.setWrapT(GL_CLAMP_TO_EDGE);

	m_geometry_fb.init();
	m_geometry_fb.attachTexture2D(m_output_position_tex.getID(), GL_COLOR_ATTACHMENT0);
	m_geometry_fb.attachTexture2D(m_output_normal_shininess_tex.getID(), GL_COLOR_ATTACHMENT1);
	m_geometry_fb.attachTexture2D(m_output_diffuse_specular_tex.getID(), GL_COLOR_ATTACHMENT2);
	m_geometry_fb.createAttachRenderbuffer(width, height);

	m_geometry_fb.drawBuffersDefault(3);
}

void GeometryStage::run()
{
	m_geometry_fb.bind();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_root_entity->draw();
}

void LightingSSAOStage::init(int width, int height, VertexArray* screen_quad, ShaderProgram* ssao_base_sp, ShaderProgram* ssao_blur_sp, GeometryStage* geometry_stage, int kernel_size, int noise_size)
{
	m_ssao_base_sp = ssao_base_sp;
	m_ssao_blur_sp = ssao_blur_sp;
	m_geometry_stage = geometry_stage;
	m_screen_quad = screen_quad;

	std::uniform_real_distribution<float> random_floats(-1.0f, 1.0f);
	std::default_random_engine			  generator;
	std::vector<glm::vec3>				  ssao_kernel(kernel_size);
	for (size_t i = 0; i < kernel_size; ++i)
	{
		ssao_kernel[i] = glm::vec3(
			random_floats(generator),
			random_floats(generator),
			(random_floats(generator) + 1.0f) * 0.5f);

		ssao_kernel[i] = glm::normalize(ssao_kernel[i]);
		ssao_kernel[i] *= (random_floats(generator) + 1.0f) * 0.5f;

		float scale = static_cast<float>(i) / kernel_size;
		scale = glm::lerp(0.1f, 1.0f, scale * scale * scale);
		ssao_kernel[i] *= scale;
	}

	std::vector<glm::vec3> ssao_noise(noise_size);
	for (size_t i = 0; i < noise_size; ++i)
	{
		ssao_noise[i] = glm::vec3(
			random_floats(generator),
			random_floats(generator),
			0.0f);
	}
	int noize_tex_size = noise_size / 4;

	m_ssao_base_sp->setVecArray("samples", ssao_kernel, kernel_size);

	m_ssao_noise_tex.init(noize_tex_size, noize_tex_size, GL_RGBA16F, 3, false);
	m_ssao_noise_tex.loadData(GL_FLOAT, GL_RGB, ssao_noise.data());
	m_ssao_noise_tex.setMinFilter(GL_NEAREST);
	m_ssao_noise_tex.setMagFilter(GL_NEAREST);
	m_ssao_noise_tex.setWrapS(GL_REPEAT);
	m_ssao_noise_tex.setWrapT(GL_REPEAT);

	m_ssao_base_tex.init(width, height, GL_R8, 1, false);
	m_ssao_base_tex.setMinFilter(GL_NEAREST);
	m_ssao_base_tex.setMagFilter(GL_NEAREST);
	m_ssao_base_tex.setWrapS(GL_CLAMP_TO_EDGE);
	m_ssao_base_tex.setWrapT(GL_CLAMP_TO_EDGE);

	m_output_ssao_blur_tex.init(width, height, GL_R8, 1, false);
	m_output_ssao_blur_tex.setMinFilter(GL_NEAREST);
	m_output_ssao_blur_tex.setMagFilter(GL_NEAREST);
	m_output_ssao_blur_tex.setWrapS(GL_CLAMP_TO_EDGE);
	m_output_ssao_blur_tex.setWrapT(GL_CLAMP_TO_EDGE);

	m_ssao_base_fb.init();
	m_ssao_base_fb.attachTexture2D(m_ssao_base_tex.getID());

	m_ssao_blur_fb.init();
	m_ssao_blur_fb.attachTexture2D(m_output_ssao_blur_tex.getID());
}

void LightingSSAOStage::run()
{
	m_geometry_stage->m_output_position_tex.bind(POSITION_UNIT);
	m_geometry_stage->m_output_normal_shininess_tex.bind(NORMAL_SHININESS_UNIT);
	m_geometry_stage->m_output_diffuse_specular_tex.bind(DIFFUSE_SPECULAR_UNIT);

	m_ssao_noise_tex.bind(SSAO_NOISE_UNIT);
	m_ssao_base_fb.bind();
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ssao_base_sp->use();
	m_screen_quad->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_ssao_base_tex.bind(SSAO_BASE_UNIT);
	m_ssao_blur_fb.bind();
	glClear(GL_COLOR_BUFFER_BIT);
	m_ssao_blur_sp->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void LightingAmbientStage::init(int width, int height, VertexArray* screen_quad, ShaderProgram* ambient_sp, GeometryStage* geometry_stage, LightingSSAOStage* ssao_stage)
{
	m_screen_quad = screen_quad;
	m_ambient_sp = ambient_sp;
	m_geometry_stage = geometry_stage;
	m_ssao_stage = ssao_stage;

	m_output_lighting_tex.init(width, height, GL_RGBA16F, 4, false);

	m_lighting_fb.init();
	m_lighting_fb.attachTexture2D(m_output_lighting_tex.getID());
	m_lighting_fb.createAttachRenderbuffer(width, height);
}

void LightingAmbientStage::run()
{
	m_geometry_stage->m_output_position_tex.bind(POSITION_UNIT);
	m_geometry_stage->m_output_normal_shininess_tex.bind(NORMAL_SHININESS_UNIT);
	m_geometry_stage->m_output_diffuse_specular_tex.bind(DIFFUSE_SPECULAR_UNIT);

	m_lighting_fb.bind();
	//m_lighting_fb.attachTexture2D(m_output_lighting_tex.getID());
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ssao_stage->m_output_ssao_blur_tex.bind(SSAO_BLUR_UNIT);
	m_screen_quad->bind();
	m_ambient_sp->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void LightingFinalStage::init(Camera* camera, ShaderProgram* diffspec_sp, GeometryStage* geometry_stage, LightingAmbientStage* ambient_stage)
{
	m_diffspec_sp = diffspec_sp;
	m_camera = camera;
	m_geometry_stage = geometry_stage;
	m_ambient_stage = ambient_stage;
	m_light_volume = generateSphere(1.0f, 18, 9);
}

void LightingFinalStage::run()
{
	m_geometry_stage->m_output_position_tex.bind(POSITION_UNIT);
	m_geometry_stage->m_output_normal_shininess_tex.bind(NORMAL_SHININESS_UNIT);
	m_geometry_stage->m_output_diffuse_specular_tex.bind(DIFFUSE_SPECULAR_UNIT);
	m_ambient_stage->m_lighting_fb.bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	m_diffspec_sp->use();
	m_diffspec_sp->setVec("camera_position", m_camera->getPosition());
	for (int i = 0; i < m_point_lights.size(); ++i)
	{
		glm::mat4 light_volume_model = glm::translate(IDENTICAL_MATRIX, m_point_lights[i].m_position);
		light_volume_model = glm::scale(light_volume_model, glm::vec3(m_point_lights[i].m_radius));
		m_diffspec_sp->setMat("light_volume_model", light_volume_model);
		m_diffspec_sp->set("current_light_index", i);
		m_light_volume.draw();
	}


	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	m_ambient_stage->m_output_lighting_tex.bind(POSTPROCESS_INPUT_UNIT);
}

void PostProcessStage::init(ShaderProgram* shader_program, VertexArray* screen_quad)
{
	m_screen_quad = screen_quad;
	m_postprocess_shader = shader_program;
}

void PostProcessStage::run()
{
	Framebuffer::bindDefault();
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	m_screen_quad->bind();
	m_postprocess_shader->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
