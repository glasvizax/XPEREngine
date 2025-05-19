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

void PrintTextureRGBA16F(GLuint textureID, int width, int height)
{
	// Привязать текстуру
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Выделить память под данные (4 float на пиксель)
	std::vector<float> pixels(width * height * 4); // RGBA

	// Считать данные из текстуры
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels.data());

	// Вывод в консоль
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int	  i = (y * width + x) * 4;
			float r = pixels[i + 0];
			float g = pixels[i + 1];
			float b = pixels[i + 2];
			float a = pixels[i + 3];

			// Отформатированный вывод
			std::cout << "("
					  << r << ", "
					  << g << ", "
					  << b << ", "
					  << a << ") ";
		}
		std::cout << std::endl;
	}

	// Отключить текстуру (по желанию)
	glBindTexture(GL_TEXTURE_2D, 0);
}



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
	m_resource_manager = &engine.getResourceManager();
	WindowManager&	 wm = engine.getWindowManager();

	glm::ivec2 window_size = wm.getWindowSize();
	glViewport(0, 0, window_size.x, window_size.y);

	m_camera.setAspectRatio(scast<float>(window_size.x) / window_size.y);
	m_camera.setPosition(glm::vec3(10.0f, 10.0f, -10.0f));
	m_camera.rotateYaw(-120);

	initMatrices();

	initScene();

	m_geometry_stage.init(window_size.x, window_size.y, *m_resource_manager);
	m_postprocess_stage.init(m_shader_program_manager.getShaderProgramPtr(ShaderProgramType::POSTPROCESS));

	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LEQUAL);
	
	checkGeneralErrorGL("render_system");
	LOG_INFO_S("bool RenderSystem::init() end");
	return true;
}

void RenderSystem::render()
{
	updateMatrices();
	m_root_entity.update();

	m_geometry_stage.run(m_root_entity);
	m_postprocess_stage.run(m_geometry_stage.m_output_diffuse_specular);
}

void RenderSystem::destroy()
{
	
}

void RenderSystem::updateMatrices()
{
	if (m_camera.isProjectionDirty())
	{
		m_matrices_buffer.fill(0, sizeof(glm::mat4), glm::value_ptr(m_camera.getProjectionMatrix()));
	}
	if (m_camera.isViewDirty())
	{
		m_matrices_buffer.fill(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera.getViewMatrix()));
	}
}

void RenderSystem::initMatrices()
{
	m_matrices_buffer.init(sizeof(glm::mat4) * 2, 0);
	m_matrices_buffer.fill(0, sizeof(glm::mat4), glm::value_ptr(m_camera.getProjectionMatrix()));
	m_matrices_buffer.fill(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera.getViewMatrix()));
}

void RenderSystem::initScene()
{
	m_root_entity.addChild();
	m_resource_manager->loadModel("content/Lowpoly_tree_sample.obj", m_root_entity.m_children.back(), false);
	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(0.4f)).setPosition(glm::vec3(10.0f, 0.0f, 10.0f));

	m_root_entity.addChild();
	m_resource_manager->loadModel("content/Cottage_FREE.obj", m_root_entity.m_children.back());

	m_root_entity.addChild();
	m_resource_manager->loadModel("content/osaka.obj", m_root_entity.m_children.back());
	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(1.0f / 25.0f)).setPosition(glm::vec3(10.0f, 0.0f, 7.0f));

	m_root_entity.addChild();
	m_resource_manager->loadModel("content/chiyo.obj", m_root_entity.m_children.back());
	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(1.0f / 25.0f)).setPosition(glm::vec3(7.0f, 0.0f, 10.0f)).setRotationEuler(glm::vec3(0.0f, -90.0f, 0.0f));

	m_root_entity.addChild();
	Mesh& cube = m_resource_manager->storeMesh(generateIdenticalCube());

	ModelEntry<MaterialColor> floor_entry;
	floor_entry.m_material.m_shader_program = m_shader_program_manager.getShaderProgramPtr(ShaderProgramType::COLOR);
	floor_entry.m_material.m_color.m_vector = glm::vec3(0.15f, 0.55f, 0.16f);
	floor_entry.m_mesh = &cube;

	m_root_entity.m_children.back().m_model.m_meshes_color.push_back(floor_entry);

	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(12.0f, 0.05f, 12.0f));
}

ShaderProgramManager& RenderSystem::getShaderProgramManager()
{
	return m_shader_program_manager;
}

Entity& RenderSystem::getRootEntity()
{
	return m_root_entity;
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
