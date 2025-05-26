#include "RenderStages.h"

#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/constants.hpp>

#include "ResourceManager.h"
#include "Entity.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"

const GLuint SKYBOX_UNIT = 5;
const GLuint DEPTHMAP_UNIT = 6;
const GLuint POSITION_UNIT = 7;
const GLuint NORMAL_SHININESS_UNIT = 8;
const GLuint DIFFUSE_SPECULAR_UNIT = 9;
const GLuint SSAO_NOISE_UNIT = 10;
const GLuint SSAO_BASE_UNIT = 11;
const GLuint SSAO_BLUR_UNIT = 12;
const GLuint POSTPROCESS_INPUT_UNIT = 13;
const GLuint POSTPROCESS_BLOOM_INPUT_UNIT = 14;

const glm::mat4 IDENTICAL_MATRIX = glm::mat4(1.0f);

float g_skybox_vertices[] = {
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

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

void LightingSSAOStage::init(
	ShaderProgram* ssao_base_sp, 
	ShaderProgram* ssao_blur_sp, 
	GeometryStage* geometry_stage, 
	VertexArray* screen_quad, 
	int power, 
	float radius_factor, 
	int blur_iterations
)
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

	m_ssao_base_sp->setVecArray("samples", ssao_kernel.data(), KERNEL_SIZE);
	m_ssao_base_sp->set("power", power);
	m_ssao_base_sp->set("radius_factor", radius_factor);
	m_ssao_blur_sp->set("blur_iterations", blur_iterations);

	m_blur_iterations = blur_iterations;
	m_radius_factor = radius_factor;
	m_power = power;

	glm::ivec2 size = geometry_stage->m_output_diffuse_specular_tex.getSize();

	m_ssao_noise_tex.init(noize_tex_size, noize_tex_size, GL_RGBA16F, 3, false);
	m_ssao_noise_tex.loadData(GL_FLOAT, GL_RGB, ssao_noise.data());
	m_ssao_noise_tex.setMinFilter(GL_NEAREST);
	m_ssao_noise_tex.setMagFilter(GL_NEAREST);
	m_ssao_noise_tex.setWrapS(GL_REPEAT);
	m_ssao_noise_tex.setWrapT(GL_REPEAT);

	m_ssao_base_tex.init(size.x, size.y, GL_R8, 1, false);
	m_ssao_base_tex.setMinFilter(GL_NEAREST);
	m_ssao_base_tex.setMagFilter(GL_NEAREST);
	m_ssao_base_tex.setWrapS(GL_CLAMP_TO_EDGE);
	m_ssao_base_tex.setWrapT(GL_CLAMP_TO_EDGE);

	m_output_ssao_blur_tex.init(size.x, size.y, GL_R8, 1, false);
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

void LightingSSAOStage::setPower(int power)
{
	m_ssao_base_sp->set("power", power);
	m_power = power;
}

void LightingSSAOStage::setRadiusFactor(float radius_factor)
{
	m_ssao_base_sp->set("radius_factor", radius_factor);
	m_radius_factor = radius_factor;
}

void LightingSSAOStage::setBlurIterations(int blur_iterations)
{
	m_ssao_blur_sp->set("blur_iterations", blur_iterations);
	m_blur_iterations = blur_iterations;
}

void LightingAmbientStage::init(ShaderProgram* ambient_sp, GeometryStage* geometry_stage, LightingSSAOStage* ssao_stage, VertexArray* screen_quad, float ambient_factor)
{
	m_screen_quad = screen_quad;
	m_ambient_sp = ambient_sp;

	glm::ivec2 size = geometry_stage->m_output_diffuse_specular_tex.getSize();

	m_output_ambient_tex.init(size.x, size.y, GL_RGBA16F, 4, false);

	m_lighting_ambient_fb.init();
	m_lighting_ambient_fb.createAttachRenderbuffer(size.x, size.y);

	m_ambient_factor = ambient_factor;
	m_ambient_sp->set("ambient_factor", ambient_factor);
}

void LightingAmbientStage::run()
{
	m_lighting_ambient_fb.bind();
	m_lighting_ambient_fb.attachTexture2D(m_output_ambient_tex);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	m_screen_quad->bind();
	m_ambient_sp->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_output_ambient_tex.bind(POSTPROCESS_INPUT_UNIT);
}

void LightingAmbientStage::setAmbientFactor(float ambient_factor)
{
	m_ambient_factor = ambient_factor;
	m_ambient_sp->set("ambient_factor", ambient_factor);
}

void LightingShadowMappingStage::init(ShaderProgram* depthmap_sp, GeometryStage* geometry_stage, int depthmap_size, float depthmap_near, float depthmap_far, std::vector<PointLight>* point_lights)
{
	glm::ivec2 size = geometry_stage->m_output_diffuse_specular_tex.getSize();

	m_depthmap_sp = depthmap_sp;
	m_main_height = size.x;
	m_main_width = size.y;
	m_root_entity = geometry_stage->m_root_entity;
	m_depthmap_far = depthmap_far;
	m_depthmap_near = depthmap_near;
	m_point_lights = point_lights;
	m_depthmap_size = depthmap_size;

	m_depthmap_sp->set("depthmap_far", depthmap_far);

	m_depthmap_fb.init();

	m_light_projection = glm::perspective(glm::half_pi<float>(), 1.0f, depthmap_near, depthmap_far);
}

void LightingShadowMappingStage::run()
{
	size_t pl_count = m_point_lights->size();

	if (m_output_depthmaps.size() < m_point_lights->size())
	{
		m_output_depthmaps.reserve(m_point_lights->size() * 2);

		for (size_t i = m_output_depthmaps.size(); i < pl_count; ++i)
		{
			Cubemap& new_cubemap = m_output_depthmaps.emplace_back();
			new_cubemap.init(m_depthmap_size, GL_DEPTH_COMPONENT24, 1, false);
			new_cubemap.setMagFilter(GL_NEAREST);
			new_cubemap.setMinFilter(GL_NEAREST);
		}
	}

	m_depthmap_fb.bind();
	glViewport(0u, 0u, m_depthmap_size, m_depthmap_size);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	m_depthmap_sp->use();
	for (int i = 0; i < m_point_lights->size(); ++i)
	{
		PointLight& current_pl = m_point_lights->at(i);

		glm::mat4 shadow_tranforms[6];
		glm::vec3 pl_position = current_pl.m_position;
		shadow_tranforms[0] = m_light_projection * glm::lookAt(pl_position, pl_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadow_tranforms[1] = m_light_projection * glm::lookAt(pl_position, pl_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadow_tranforms[2] = m_light_projection * glm::lookAt(pl_position, pl_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shadow_tranforms[3] = m_light_projection * glm::lookAt(pl_position, pl_position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		shadow_tranforms[4] = m_light_projection * glm::lookAt(pl_position, pl_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadow_tranforms[5] = m_light_projection * glm::lookAt(pl_position, pl_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		
		m_depthmap_fb.attachCubemap(m_output_depthmaps[i], GL_DEPTH_ATTACHMENT);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_depthmap_sp->set("current_light_index", i);
		m_depthmap_sp->setMatArray("shadow_tranforms", shadow_tranforms, 6);
		m_root_entity->drawCustom(m_depthmap_sp);
	}
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
	
	glViewport(0u, 0u, m_main_width, m_main_height);
}

void LightingFinalStage::init(ShaderProgram* diffspec_sp, LightingAmbientStage* ambient_stage, LightingShadowMappingStage* shadow_mapping_stage, std::vector<PointLight>* point_lights, Camera* camera)
{
	m_diffspec_sp = diffspec_sp;
	m_camera = camera;
	m_lighting_final_fb = &ambient_stage->m_lighting_ambient_fb;
	m_output_lighting_tex = &ambient_stage->m_output_ambient_tex;
	m_input_depthmaps = &shadow_mapping_stage->m_output_depthmaps;
	m_point_lights = point_lights;
	m_diffspec_sp->set("depthmap_far", shadow_mapping_stage->m_depthmap_far);
	
	m_light_volume = generateSphere(1.0f, 18, 9);
}

void LightingFinalStage::run()
{
	m_lighting_final_fb->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	m_diffspec_sp->use();
	m_diffspec_sp->setVec("camera_position", m_camera->getPosition());
	for (int i = 0; i < m_point_lights->size(); ++i)
	{
		PointLight& current_pl = m_point_lights->at(i);
		glm::mat4	light_volume_model = glm::translate(IDENTICAL_MATRIX, current_pl.m_position);
		light_volume_model = glm::scale(light_volume_model, glm::vec3(current_pl.m_radius));
		m_diffspec_sp->setMat("light_volume_model", light_volume_model);
		m_diffspec_sp->set("current_light_index", i);
		m_input_depthmaps->at(i).bind(DEPTHMAP_UNIT);
		m_light_volume.draw();
	}


	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void ForwardStage::init(ShaderProgram* forward_color_sp, GeometryStage* geometry_stage, LightingFinalStage* lighting_final_stage)
{
	m_forward_color_sp = forward_color_sp;
	m_output_tex = lighting_final_stage->m_output_lighting_tex;
	m_point_lights = lighting_final_stage->m_point_lights;
	m_geomentry_rb = &geometry_stage->m_geometry_rb;
	m_light_sphere = generateSphere(1.0f, 36, 18);
	m_forward_fb.init();
}

void ForwardStage::run()
{
	m_forward_fb.bind();
	m_forward_fb.attachTexture2D(*m_output_tex);
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
}

void SkyboxStage::init(ShaderProgram* skybox_sp, ForwardStage* forward_stage, GeometryStage* geometry_stage, Cubemap* skybox_cm, Camera* camera)
{
	m_skybox_sp = skybox_sp;
	m_camera = camera;
	m_output_tex = forward_stage->m_output_tex;
	m_input_rb = &geometry_stage->m_geometry_rb;

	m_skybox_fb.init();

	m_skybox_va.init();
	m_skybox_va.attachArrayBuffer(sizeof(g_skybox_vertices), g_skybox_vertices);
	m_skybox_va.enableAttribute(0, 3, 3, 0);
	
	m_skybox_cm = skybox_cm;
}

void SkyboxStage::run()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_skybox_fb.bind();

	m_skybox_fb.attachRenderbuffer(std::move(*m_input_rb));
	m_skybox_fb.attachTexture2D(*m_output_tex);

	m_skybox_sp->use();
	m_skybox_sp->setMat("skybox_view", glm::mat4(glm::mat3(m_camera->getViewMatrix())));

	m_skybox_cm->bind(SKYBOX_UNIT);
	m_skybox_va.bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	*m_input_rb = std::move(m_skybox_fb.m_renderbuffer);

	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);

	m_output_tex->bind(POSTPROCESS_INPUT_UNIT);
}

void SkyboxStage::setSkyboxCubemap(Cubemap* skybox_cm)
{
	m_skybox_cm = skybox_cm;
}

void BloomStage::init(ShaderProgram* brightness_extraction, ShaderProgram* blur, SkyboxStage* skybox_stage, VertexArray* screen_quad, float threshold)
{
	m_brightness_extraction_sp = brightness_extraction;
	m_blur_sp = blur;
	m_input_tex = skybox_stage->m_output_tex;
	m_screen_quad = screen_quad;

	m_brightness_extraction_sp->set("threshold", threshold);

	glm::ivec2 size = m_input_tex->getSize();
	m_brightness_extraction_tex.init(size.x, size.y, GL_RGBA16F, 4, false);
	m_brightness_extraction_tex.setWrapS(GL_CLAMP_TO_EDGE);
	m_brightness_extraction_tex.setWrapT(GL_CLAMP_TO_EDGE);

	m_brightness_extraction_fb.init();

	for (int i = 0; i < 2; ++i)
	{
		m_blur_pingpong_fbs[i].init();
		m_blur_pingpong_texs[i].init(size.x, size.y, GL_RGBA16F, 4, false);
		m_blur_pingpong_texs[i].setWrapS(GL_CLAMP_TO_EDGE);
		m_blur_pingpong_texs[i].setWrapT(GL_CLAMP_TO_EDGE);
		m_blur_pingpong_fbs[i].attachTexture2D(m_blur_pingpong_texs[i]);
	}
}

void BloomStage::run()
{
	m_brightness_extraction_fb.bind();
	glDisable(GL_DEPTH_TEST);
	m_brightness_extraction_fb.attachTexture2D(m_brightness_extraction_tex);
	glClear(GL_COLOR_BUFFER_BIT);
	m_brightness_extraction_sp->use();
	m_screen_quad->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_blur_sp->use();
	bool horizontal = true;
	uint amount = 27u;

	m_blur_pingpong_fbs[horizontal].bind();
	glClear(GL_COLOR_BUFFER_BIT); 
	m_blur_sp->set("horizontal", horizontal);
	m_brightness_extraction_tex.bind(POSTPROCESS_BLOOM_INPUT_UNIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	horizontal = !horizontal;

	for (uint i = 1u; i < amount; ++i)
	{
		m_blur_pingpong_fbs[horizontal].bind();
		glClear(GL_COLOR_BUFFER_BIT);
		m_blur_sp->set("horizontal", horizontal);
		m_blur_pingpong_texs[!horizontal].bind(POSTPROCESS_BLOOM_INPUT_UNIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		horizontal = !horizontal;
	}

	m_blur_pingpong_texs[!horizontal].bind(POSTPROCESS_BLOOM_INPUT_UNIT);
}

void BloomStage::setThreshold(float threshold)
{
	m_threshold = threshold;
	m_brightness_extraction_sp->set("threshold", threshold);
}

void PostProcessStage::init(ShaderProgram* shader_program, VertexArray* screen_quad)
{
	m_screen_quad = screen_quad;
	m_postprocess_shader = shader_program;
}

void PostProcessStage::run()
{
	Framebuffer::bindDefault();
	glClear(GL_COLOR_BUFFER_BIT);

	m_screen_quad->bind();
	m_postprocess_shader->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
