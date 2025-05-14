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

float vertices[] = 
{
	-0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
	0.0f, 0.5f,			0.0f, 1.0f, 0.0f,
	0.5f, -0.5f,		0.0f, 0.0f, 1.0f
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

	Engine& engine = Engine::getInstance();
	ResourceManager& rm = engine.getResourceManager();
	WindowManager& wm = engine.getWindowManager();

	glGenBuffers(1, &m_matrices_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_matrices_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_matrices_ubo);

	if (!rm.initLoadShaderProgram("test.vert", "test.frag", m_color_program))
	{
		return false;
	}

	glm::ivec2 window_size = wm.getWindowSize();
	glViewport(0, 0, window_size.x, window_size.y);
	
	m_camera.setAspectRatio(scast<float>(window_size.x) / window_size.y);

	glBindBuffer(GL_UNIFORM_BUFFER, m_matrices_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_camera.getProjectionMatrix()));

	checkGeneralErrorGL("render_system");

	m_cube_mesh = generateIdenticalCube();
	m_color_material.m_shader_program = &m_color_program;

	Model model;
	ModelEntry<MaterialC> model_entry{ &m_cube_mesh, &m_color_material };

	model.color_entries.emplace_back(model_entry);
	
	m_scene_root.addChild(model);
	//m_scene_root.m_children.front().addChild()

	LOG_INFO_S("bool RenderSystem::init() end");

	return true;
}

void RenderSystem::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, m_matrices_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera.getViewMatrix()));
	
	m_scene_root.update();

	m_scene_root.draw();
	
}

void RenderSystem::destroy()
{

}

Camera& RenderSystem::getCamera()
{
	return m_camera;
}
