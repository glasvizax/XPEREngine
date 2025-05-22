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
#include "Utils.h"
#include "Light.h"

int getUniformBlockSize(ShaderProgram& shader, const std::string& name);

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

	Engine& engine = Engine::getInstance();
	WindowManager& wm = engine.getWindowManager();
	m_resource_manager = &engine.getResourceManager();

	glm::ivec2 window_size = wm.getWindowSize();
	glViewport(0, 0, window_size.x, window_size.y);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_camera.setAspectRatio(scast<float>(window_size.x) / window_size.y);
	m_camera.setPosition(glm::vec3(7.8f, 1.0f, -8.5f));
	m_camera.rotateYaw(-120);

	initMatricesBuffer();
	initLightsBuffer();
	initScene();
	initScreenQuad();

	std::vector<std::string> skybox_files = {
		"content/textures/Daylight Box_Right.bmp",
		"content/textures/Daylight Box_Left.bmp",
		"content/textures/Daylight Box_Top.bmp",
		"content/textures/Daylight Box_Bottom.bmp",
		"content/textures/Daylight Box_Front.bmp",
		"content/textures/Daylight Box_Back.bmp"
	};
	Cubemap skybox_cm;
	m_resource_manager->initLoadCubemap(skybox_files, skybox_cm, false);


	ShaderProgram* ssao_base = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_SSAO_BASE);
	ShaderProgram* ssao_blur = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_SSAO_BLUR);
	ShaderProgram* ambient = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_AMBIENT);
	ShaderProgram* diffspec = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR);
	ShaderProgram* depthmap = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_DEPTHMAP);
	ShaderProgram* forward_color = m_shader_program_manager.getShaderProgram(ShaderProgramType::FORWARD_COLOR);
	ShaderProgram* postprocess = m_shader_program_manager.getShaderProgram(ShaderProgramType::POSTPROCESS);
	ShaderProgram* skybox = m_shader_program_manager.getShaderProgram(ShaderProgramType::SKYBOX);

	

	m_geometry_stage.init(window_size.x, window_size.y, &m_root_entity);
	m_ssao_stage.init(ssao_base, ssao_blur, window_size.x, window_size.y, &m_screen_quad);
	m_ambient_stage.init(ambient, &m_geometry_stage, &m_ssao_stage, window_size.x, window_size.y, &m_screen_quad);
	m_shadow_mapping_stage.init(depthmap, window_size.x, window_size.y, 2048, 1.0f, 100.0f, &m_root_entity, &m_point_lights);
	m_final_stage.init(diffspec, &m_ambient_stage, &m_shadow_mapping_stage, &m_point_lights, &m_camera);
	m_forward_stage.init(forward_color, &m_geometry_stage, &m_final_stage);
	m_skybox_stage.init(skybox, &m_forward_stage, &m_geometry_stage, std::move(skybox_cm), &m_camera);
	m_postprocess_stage.init(postprocess, &m_screen_quad);

	PointLight pl1;
	pl1.m_position = glm::vec3(10.0f, 3.0f, 5.0f);
	pl1.m_ambient = glm::vec3(0.2f);
	pl1.m_diffuse = glm::vec3(0.5f);
	pl1.m_specular = glm::vec3(0.6f);
	pl1.m_linear = 0.027f;
	pl1.m_quadratic = 0.0028f;

	PointLight pl2;
	pl2.m_position = glm::vec3(7.0f, 3.0f, 2.0f);
	pl2.m_ambient = glm::vec3(0.1f);
	pl2.m_diffuse = glm::vec3(0.5f, 0.2f, 0.3f);
	pl2.m_specular = glm::vec3(0.1f);
	pl2.m_linear = 0.027f;
	pl2.m_quadratic = 0.0028f;

	addPointLight(pl1);
	addPointLight(pl2);

	LOG_INFO_F("Light size = [%d]", getUniformBlockSize(*diffspec, "Lights"));

	checkGeneralErrorGL("render_system");
	LOG_INFO_S("bool RenderSystem::init() end");
	return true;
}

void RenderSystem::render()
{
	updateMatrices();
	m_root_entity.update();

	m_geometry_stage.run();
	m_ssao_stage.run();
	m_ambient_stage.run();
	m_shadow_mapping_stage.run();
	m_final_stage.run();
	m_forward_stage.run();
	m_skybox_stage.run();
	m_postprocess_stage.run();
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

void RenderSystem::initMatricesBuffer()
{
	m_matrices_buffer.init(sizeof(glm::mat4) * 2, 0);
	m_matrices_buffer.fill(0, sizeof(glm::mat4), glm::value_ptr(m_camera.getProjectionMatrix()));
	m_matrices_buffer.fill(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera.getViewMatrix()));
}

void RenderSystem::initLightsBuffer()
{
	m_lights_buffer.init(sizeof(PointLight) * MAX_POINT_LIGHTS + sizeof(glm::vec4), 1);
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
	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(1.0f / 35.0f)).setPosition(glm::vec3(7.0f, 0.0f, 10.0f)).setRotationEuler(glm::vec3(0.0f, -90.0f, 0.0f));

	m_root_entity.addChild();
	Mesh& cube = m_resource_manager->storeMesh(generateIdenticalCube());

	ModelEntry<MaterialColor> floor_entry;
	floor_entry.m_material.m_shader_program = m_shader_program_manager.getShaderProgram(ShaderProgramType::DEFERED_COLOR);
	floor_entry.m_material.m_color.m_vector = glm::vec3(0.3f, 0.8f, 0.2f);
	floor_entry.m_material.m_shininess.m_scalar = 32.0f;
	floor_entry.m_material.m_specular_scalar.m_scalar = 0.5f;

	floor_entry.m_mesh = &cube;

	m_root_entity.m_children.back().m_model.m_meshes_color.push_back(floor_entry);

	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(20.0f, 0.05f, 20.0f));
}

void RenderSystem::initScreenQuad()
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

void RenderSystem::addPointLight(PointLight& point_light)
{
	int size = m_point_lights.size();
	if (size > MAX_POINT_LIGHTS)
	{
		LOG_WARNING_S("size > MAX_POINT_LIGHTS");
		return;
	}

	float light_max = std::fmaxf(std::fmaxf(point_light.m_diffuse.r, point_light.m_diffuse.g), point_light.m_diffuse.b);
	point_light.m_radius = (-point_light.m_linear + std::sqrtf(point_light.m_linear * point_light.m_linear - 4 * point_light.m_quadratic * (1.0f - (256.0 / 5.0) * light_max))) / (2 * point_light.m_quadratic);

	m_lights_buffer.fill(size * sizeof(PointLight), sizeof(PointLight), &point_light);
	size++;
	m_lights_buffer.fill(MAX_POINT_LIGHTS * sizeof(PointLight), sizeof(int), &size);
	m_point_lights.push_back(point_light);
}

int getUniformBlockSize(ShaderProgram& shader, const std::string& name)
{
	GLuint index = glGetUniformBlockIndex(shader.getID(), name.c_str());
	if (index == GL_INVALID_INDEX)
	{
		return 0;
	}
	else
	{
		int sz = 0;
		glGetActiveUniformBlockiv(shader.getID(), index, GL_UNIFORM_BLOCK_DATA_SIZE, &sz);
		return sz;
	}
}
