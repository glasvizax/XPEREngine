#include "RenderSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Defines.h"
#include "Debug.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "DebugOpenGL.h"
#include "WindowManager.h"
#include "Mesh.h"

float vertices[] = {
	-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

bool RenderSystem::init()
{
	if (!gladLoadGLLoader(rcast<GLADloadproc>(glfwGetProcAddress)))
	{
		LOG_ERROR_S("couldn't load glad loader");
		return false;
	}

#ifdef _DEBUG

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

#endif // DEBUG

	m_shader_program_manager.init();

	Engine&			 engine = Engine::getInstance();
	ResourceManager& rm = engine.getResourceManager();
	WindowManager&	 wm = engine.getWindowManager();

	glm::ivec2 window_size = wm.getWindowSize();
	glViewport(0, 0, window_size.x, window_size.y);

	m_camera.setAspectRatio(scast<float>(window_size.x) / window_size.y);

	checkGeneralErrorGL("render_system");

	//rm.loadModel("content/backpack.obj", m_root_entity);
	rm.loadModel("content/scene.gltf", m_root_entity);

	/*
	if (!rm.initLoadTexture("content/wood.jpg", wood_tex, true))
	{
		return false;
	}
	
	m_cube_mesh = generateIdenticalCube();
	
	Model model;

	ModelEntry<MaterialColor> model_entry;
	model_entry.m_material.m_shader_program = m_shader_program_manager.getShaderProgramPtr(ShaderProgramType::COLOR);
	model_entry.m_material.m_color.m_value = glm::vec4(0.3f, 0.6f, 0.9f, 1.0f);
	model_entry.m_mesh = &m_cube_mesh;
	model.m_color_meshes.emplace_back(model_entry);

	Model				  model2;
	ModelEntry<MaterialDiff> model2_entry;
	model2_entry.m_mesh = &m_cube_mesh;
	model2_entry.m_material.m_diffuse.pushDiffuse(&wood_tex);
	model2_entry.m_material.m_shader_program = m_shader_program_manager.getShaderProgramPtr(ShaderProgramType::DIFFUSE);
	model2.m_diff_meshes.emplace_back(model2_entry);

	Transform transform;
	transform.setPosition(glm::vec3(2.0f));

	m_root_entity.addChild(model);
	m_root_entity.m_children.front().addChild(model2, transform);
	*/

	glGenBuffers(1, &m_matrices_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_matrices_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_matrices_ubo);

	glBindBuffer(GL_UNIFORM_BUFFER, m_matrices_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_camera.getProjectionMatrix()));

	glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LEQUAL);
	
	LOG_INFO_S("bool RenderSystem::init() end");
	return true;
}

void RenderSystem::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, m_matrices_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera.getViewMatrix()));

	m_root_entity.update();
	m_root_entity.draw();
}

void RenderSystem::destroy()
{
}

Camera& RenderSystem::getCamera()
{
	return m_camera;
}

void RenderSystem::testInputH()
{
	Transform& tranform = m_root_entity.m_children.front().getTransform();
	tranform.setPosition(tranform.getPosition() + glm::vec3(0.5f, 0.0f, 0.0f));
}

void RenderSystem::testInputK()
{
}
