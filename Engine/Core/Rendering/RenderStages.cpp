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

	m_geometry_rb.init();
	m_geometry_rb.setStorage(width, height, GL_DEPTH24_STENCIL8);

	m_geometry_fb.init();
	m_geometry_fb.attachTexture2D(m_output_position_tex, GL_COLOR_ATTACHMENT0);
	m_geometry_fb.attachTexture2D(m_output_normal_shininess_tex, GL_COLOR_ATTACHMENT1);
	m_geometry_fb.attachTexture2D(m_output_diffuse_specular_tex, GL_COLOR_ATTACHMENT2);
	m_geometry_fb.drawBuffersDefault(3);

	m_output_position_tex.bind(POSITION_UNIT);
	m_output_normal_shininess_tex.bind(NORMAL_SHININESS_UNIT);
	m_output_diffuse_specular_tex.bind(DIFFUSE_SPECULAR_UNIT);
}

void GeometryStage::run()
{
	m_geometry_fb.bind();
	m_geometry_fb.attachRenderbuffer(std::move(m_geometry_rb));
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_root_entity->draw();
	m_geometry_rb = std::move(m_geometry_fb.m_renderbuffer);
}

void LightingSSAOStage::init(int width, int height, VertexArray* screen_quad, ShaderProgram* ssao_base_sp, ShaderProgram* ssao_blur_sp)
{
	m_ssao_base_sp = ssao_base_sp;
	m_ssao_blur_sp = ssao_blur_sp;
	m_screen_quad = screen_quad;

	std::uniform_real_distribution<float> random_floats(-1.0f, 1.0f);
	std::default_random_engine			  generator;
	std::vector<glm::vec3>				  ssao_kernel(KERNEL_SIZE);
	for (size_t i = 0; i < KERNEL_SIZE; ++i)
	{
		ssao_kernel[i] = glm::vec3(
			random_floats(generator),
			random_floats(generator),
			(random_floats(generator) + 1.0f) * 0.5f);

		ssao_kernel[i] = glm::normalize(ssao_kernel[i]);
		ssao_kernel[i] *= (random_floats(generator) + 1.0f) * 0.5f;

		float scale = static_cast<float>(i) / KERNEL_SIZE;
		scale = glm::lerp(0.1f, 1.0f, scale * scale * scale);
		ssao_kernel[i] *= scale;
	}

	std::vector<glm::vec3> ssao_noise(NOISE_SIZE);
	for (size_t i = 0; i < NOISE_SIZE; ++i)
	{
		ssao_noise[i] = glm::vec3(
			random_floats(generator),
			random_floats(generator),
			0.0f);
	}
	int noize_tex_size = NOISE_SIZE / 4;

	m_ssao_base_sp->setVecArray("samples", ssao_kernel, KERNEL_SIZE);

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
	m_ssao_base_fb.attachTexture2D(m_ssao_base_tex);

	m_ssao_blur_fb.init();
	m_ssao_blur_fb.attachTexture2D(m_output_ssao_blur_tex);
}

void LightingSSAOStage::run()
{
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
	m_output_ssao_blur_tex.bind(SSAO_BLUR_UNIT);
}

void LightingAmbientStage::init(int width, int height, VertexArray* screen_quad, ShaderProgram* ambient_sp, GeometryStage* geometry_stage, LightingSSAOStage* ssao_stage)
{
	m_screen_quad = screen_quad;
	m_ambient_sp = ambient_sp;

	m_output_ambient_tex.init(width, height, GL_RGBA16F, 4, false);

	m_lighting_fb.init();
	m_lighting_fb.createAttachRenderbuffer(width, height);
}

void LightingAmbientStage::run()
{
	m_lighting_fb.bind();
	m_lighting_fb.attachTexture2D(m_output_ambient_tex);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	m_screen_quad->bind();
	m_ambient_sp->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_output_ambient_tex.bind(POSTPROCESS_INPUT_UNIT);
}

void LightingFinalStage::init(Camera* camera, ShaderProgram* diffspec_sp, LightingAmbientStage* ambient_stage)
{
	m_diffspec_sp = diffspec_sp;
	m_camera = camera;
	m_output_lighting_tex = &ambient_stage->m_output_ambient_tex;
	m_light_volume = generateSphere(1.0f, 18, 9);
}

void LightingFinalStage::run()
{
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
}

void ForwardStage::init(ShaderProgram* forward_color_sp, GeometryStage* geometry_stage, LightingFinalStage* lighting_final_stage)
{
	m_forward_color_sp = forward_color_sp;
	m_input_lighting_tex = lighting_final_stage->m_output_lighting_tex;
	m_point_lights = &lighting_final_stage->m_point_lights;
	m_geomentry_rb = &geometry_stage->m_geometry_rb;
	m_light_sphere = generateSphere(1.0f, 36, 18);
	m_forward_fb.init();
}

void ForwardStage::run()
{
	m_forward_fb.bind();
	m_forward_fb.attachTexture2D(*m_input_lighting_tex);
	m_forward_fb.attachRenderbuffer(std::move(*m_geomentry_rb));
	glEnable(GL_DEPTH_TEST);
	m_forward_color_sp->use();
	for (int i = 0; i < m_point_lights->size(); ++i)
	{
		PointLight& current_pl = m_point_lights->at(i);
		glm::mat4	model = glm::translate(IDENTICAL_MATRIX, current_pl.m_position);
		model = glm::scale(model, glm::vec3(0.25f));
		m_forward_color_sp->setMat("model", model);
		m_forward_color_sp->setVec("material.color", current_pl.m_diffuse);
		m_light_sphere.draw();
	}

	*m_geomentry_rb = std::move(m_forward_fb.m_renderbuffer);
	m_input_lighting_tex->bind(POSTPROCESS_INPUT_UNIT);
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