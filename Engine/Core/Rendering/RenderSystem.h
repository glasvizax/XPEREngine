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
#include "Framebuffer.h"

class GeometryStage
{
public:
	void init(int width, int height, ResourceManager& resource_manager)
	{
		Texture position;
		position.init(width, height, GL_RGBA16F, 4u, false);
		Texture normal_shininess;
		normal_shininess.init(width, height, GL_RGBA16F, 4u, false);
		Texture diffuse_specular;
		diffuse_specular.init(width, height, GL_RGBA16F, 4u, false);

		m_deferred_framebuffer.init();
		m_deferred_framebuffer.attachTexture2D(position.getID(), GL_COLOR_ATTACHMENT0);
		m_deferred_framebuffer.attachTexture2D(normal_shininess.getID(), GL_COLOR_ATTACHMENT1);
		m_deferred_framebuffer.attachTexture2D(diffuse_specular.getID(), GL_COLOR_ATTACHMENT2);
		m_deferred_framebuffer.createAttachRenderbuffer(width, height);

		m_output_position = &resource_manager.storeTexture(std::move(position));
		m_output_normal_shininess = &resource_manager.storeTexture(std::move(normal_shininess));
		m_output_diffuse_specular = &resource_manager.storeTexture(std::move(diffuse_specular));

		m_deferred_framebuffer.drawBuffersDefault(3);
	}

	void run(Entity& root)
	{

		m_deferred_framebuffer.bind();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		root.draw();
	}

	Texture* m_output_position;
	Texture* m_output_normal_shininess;
	Texture* m_output_diffuse_specular;

	Framebuffer m_deferred_framebuffer;
};

class PostProcessStage
{
public:
	void init(ShaderProgram* shader_program)
	{
		float quad[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		};
		m_screen_quad.init();
		m_screen_quad.attachArrayBuffer(sizeof(quad), quad);
		m_screen_quad.enableAttribute(0, 2, 4, 0);
		m_screen_quad.enableAttribute(1, 2, 4, 2);
		m_postprocess_shader = shader_program;
	}

	void run(Texture* input_texture)
	{
		Framebuffer::bindDefault();
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		input_texture->bind(m_input_unit);
		m_screen_quad.bind();
		m_postprocess_shader->use();
		m_postprocess_shader->set("input_texture", 12);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	VertexArray	   m_screen_quad;
	ShaderProgram* m_postprocess_shader;

	const GLuint m_input_unit = 12u;
};

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

private:
	void updateMatrices();
	void initMatrices();
	void initScene();

private:
	ResourceManager* m_resource_manager;

	Entity m_root_entity;

	ShaderProgramManager m_shader_program_manager;

	Camera m_camera;

	UniformBuffer m_matrices_buffer;

	GeometryStage	 m_geometry_stage;
	PostProcessStage m_postprocess_stage;
};
