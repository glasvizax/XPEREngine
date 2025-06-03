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
	m_camera.rotateYaw(-160);

	initMatricesBuffer();
	initLightsBuffer();
	initScene();
	initScreenQuad();

	std::vector<std::string> skybox_daylight_files = {
		"content/textures/skybox_daylight/skybox_daylight1.bmp",
		"content/textures/skybox_daylight/skybox_daylight2.bmp",
		"content/textures/skybox_daylight/skybox_daylight3.bmp",
		"content/textures/skybox_daylight/skybox_daylight4.bmp",
		"content/textures/skybox_daylight/skybox_daylight5.bmp",
		"content/textures/skybox_daylight/skybox_daylight6.bmp"
	};
	m_resource_manager->initLoadCubemap(skybox_daylight_files, m_daylight, false);

	std::vector<std::string> skybox_night_files = {
		"content/textures/skybox_night/skybox_night1.png",
		"content/textures/skybox_night/skybox_night2.png",
		"content/textures/skybox_night/skybox_night3.png",
		"content/textures/skybox_night/skybox_night4.png",
		"content/textures/skybox_night/skybox_night5.png",
		"content/textures/skybox_night/skybox_night6.png"
	};
	m_resource_manager->initLoadCubemap(skybox_night_files, m_night, false);

	ShaderProgram* ssao_base = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_SSAO_BASE);
	ShaderProgram* ssao_blur = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_SSAO_BLUR);
	ShaderProgram* ambient = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_AMBIENT);
	ShaderProgram* diffspec = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_DIFFUSE_SPECULAR);
	ShaderProgram* depthmap = m_shader_program_manager.getShaderProgram(ShaderProgramType::LIGHTING_DEPTHMAP);
	ShaderProgram* forward_color = m_shader_program_manager.getShaderProgram(ShaderProgramType::FORWARD_COLOR);
	ShaderProgram* postprocess = m_shader_program_manager.getShaderProgram(ShaderProgramType::POSTPROCESS);
	ShaderProgram* bloom_brightness_extraction = m_shader_program_manager.getShaderProgram(ShaderProgramType::BLOOM_BRIGHTNESS_EXTRACTION);
	ShaderProgram* bloom_blur = m_shader_program_manager.getShaderProgram(ShaderProgramType::BLOOM_BLUR);
	ShaderProgram* skybox = m_shader_program_manager.getShaderProgram(ShaderProgramType::SKYBOX);

	m_geometry_stage.init(window_size.x, window_size.y, &m_root_entity);
	m_ssao_stage.init(ssao_base, ssao_blur, &m_geometry_stage, &m_screen_quad);
	m_ambient_stage.init(ambient, &m_geometry_stage, &m_ssao_stage, &m_screen_quad);
	m_shadow_mapping_stage.init(depthmap, &m_geometry_stage, 2048, 1.0f, 100.0f, &m_point_lights);
	m_lighting_final_stage.init(diffspec, &m_ambient_stage, &m_shadow_mapping_stage, &m_point_lights, &m_camera);
	m_forward_stage.init(forward_color, &m_geometry_stage, &m_lighting_final_stage);
	m_skybox_stage.init(skybox, &m_forward_stage, &m_geometry_stage, &m_night, &m_camera);
	m_bloom_stage.init(bloom_brightness_extraction, bloom_blur, &m_skybox_stage, &m_screen_quad);
	m_postprocess_stage.init(postprocess, &m_screen_quad);

	m_point_lights.reserve(MAX_POINT_LIGHTS);
	PointLight pl1;
	pl1.m_position = glm::vec3(10.0f, 3.0f, 5.0f);
	pl1.m_ambient = glm::vec3(0.2f);
	pl1.m_diffuse = glm::vec3(0.5f);
	pl1.m_specular = glm::vec3(0.6f);
	pl1.m_linear = 0.027f;
	pl1.m_quadratic = 0.0028f;
	
	PointLight pl2;
	pl2.m_position = glm::vec3(7.0f, 10.0f, 2.0f);
	pl2.m_ambient = glm::vec3(0.1f);
	pl2.m_diffuse = glm::vec3(1.0f);
	pl2.m_specular = glm::vec3(0.3f);
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
	m_lighting_final_stage.run();
	m_forward_stage.run();
	m_skybox_stage.run();
	m_bloom_stage.run();
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
	m_resource_manager->loadModel("content/Lowpoly_tree_sample.obj", m_root_entity.m_children.back());
	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(0.4f)).setPosition(glm::vec3(10.0f, 0.0f, 10.0f));

	m_root_entity.addChild();
	m_resource_manager->loadModel("content/Cottage_FREE.obj", m_root_entity.m_children.back(), true);

	m_root_entity.addChild();
	m_resource_manager->loadModel("content/osaka.obj", m_root_entity.m_children.back());
	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(1.0f / 30.0f)).setPosition(glm::vec3(10.0f, 0.0f, 7.0f));

	m_root_entity.addChild();
	m_resource_manager->loadModel("content/chiyo.obj", m_root_entity.m_children.back());
	m_root_entity.m_children.back().getTransform().setScale(glm::vec3(1.0f / 40.0f)).setPosition(glm::vec3(7.0f, 0.0f, 10.0f)).setRotationEuler(glm::vec3(0.0f, -90.0f, 0.0f));

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
	int random_x = rand() % 20 - 10;
	int random_z = rand() % 20 - 10;

	PointLight pl;

	pl.m_position = glm::vec3(random_x, 5.0f, random_z);
	pl.m_ambient = glm::vec3(0.1f);
	pl.m_diffuse = glm::vec3(0.2f);
	pl.m_specular = glm::vec3(0.01f);
	pl.m_linear = 0.027f;
	pl.m_quadratic = 0.0028f;

	addPointLight(pl);
}

void RenderSystem::testInputK()
{
	uint last = m_point_lights.size() - 1;
	glm::vec3 diff = getPointLightDiffuse(last);
	diff.x += 1.0f;
	setPointLightDiffuse(last, diff);
}

uint RenderSystem::addPointLight(PointLight& point_light)
{
	int size = m_point_lights.size();
	if (size > MAX_POINT_LIGHTS)
	{
		LOG_WARNING_S("size > MAX_POINT_LIGHTS");
		return 0u;
	}

	float light_max = std::fmaxf(std::fmaxf(point_light.m_diffuse.r, point_light.m_diffuse.g), point_light.m_diffuse.b);
	point_light.m_radius = (-point_light.m_linear + std::sqrtf(point_light.m_linear * point_light.m_linear - 4 * point_light.m_quadratic * (1.0f - (256.0 / 5.0) * light_max))) / (2 * point_light.m_quadratic);

	m_lights_buffer.fill(size * sizeof(PointLight), sizeof(PointLight), &point_light);
	size++;
	m_lights_buffer.fill(MAX_POINT_LIGHTS * sizeof(PointLight), sizeof(int), &size);
	m_point_lights.push_back(point_light);
	return m_point_lights.size() - 1;
}

void RenderSystem::deletePointLight(uint light_index)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_WARNING_S("light_index should be less than size");
	}

	int last_index = m_point_lights.size() - 1;

	if (light_index == last_index)
	{
		m_point_lights.pop_back();
		m_lights_buffer.fill(MAX_POINT_LIGHTS * sizeof(PointLight), sizeof(int), &last_index);
	}
	else
	{
		m_point_lights[light_index] = m_point_lights.back();
		m_point_lights.pop_back();
		m_lights_buffer.fill(MAX_POINT_LIGHTS * sizeof(PointLight), sizeof(int), &last_index);
	}
}

void RenderSystem::setPointLightPosition(uint light_index, glm::vec3 new_position)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return;
	}

	m_point_lights[light_index].m_position = new_position;
	m_lights_buffer.fill(
		light_index * sizeof(PointLight) + offsetof(PointLight, m_position),
		member_size(PointLight, m_position),
		&new_position
	);
}

glm::vec3 RenderSystem::getPointLightPosition(uint light_index)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return glm::vec3(0.0f);
	}
	return m_point_lights[light_index].m_position;
}

void RenderSystem::setPointLightDiffuse(uint light_index, glm::vec3 new_diffuse)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return;
	}

	m_point_lights[light_index].m_diffuse = new_diffuse;
	m_lights_buffer.fill(
		light_index * sizeof(PointLight) + offsetof(PointLight, m_diffuse),
		member_size(PointLight, m_diffuse),
		&new_diffuse
	);
}

glm::vec3 RenderSystem::getPointLightDiffuse(uint light_index)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return glm::vec3(0.0f);
	}
	return m_point_lights[light_index].m_diffuse;
}

void RenderSystem::setPointLightSpecular(uint light_index, glm::vec3 new_specular)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return;
	}

	m_point_lights[light_index].m_specular = new_specular;
	m_lights_buffer.fill(
		light_index * sizeof(PointLight) + offsetof(PointLight, m_specular), 
		member_size(PointLight, m_specular), 
		&new_specular.r
	);
}

glm::vec3 RenderSystem::getPointLightSpecular(uint light_index)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return glm::vec3(0.0f);
	}
	return m_point_lights[light_index].m_specular;
}

void RenderSystem::setPointLightAmbient(uint light_index, glm::vec3 new_ambient)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return;
	}

	m_point_lights[light_index].m_ambient = new_ambient;
	m_lights_buffer.fill(
		light_index * sizeof(PointLight) + offsetof(PointLight, m_ambient),
		member_size(PointLight, m_ambient),
		&new_ambient
	);
}

glm::vec3 RenderSystem::getPointLightAmbient(uint light_index)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return glm::vec3(0.0f);
	}
	return m_point_lights[light_index].m_ambient;
}

void RenderSystem::setPointLightLinear(uint light_index, float new_linear)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return;
	}

	m_point_lights[light_index].m_linear = new_linear;
	m_lights_buffer.fill(
		light_index * sizeof(PointLight) + offsetof(PointLight, m_linear),
		member_size(PointLight, m_linear),
		&new_linear);
}

float RenderSystem::getPointLightLinear(uint light_index)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return 0.0f;
	}
	return m_point_lights[light_index].m_linear;
}

void RenderSystem::setPointLightQuadratic(uint light_index, float new_quadratic)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return;
	}

	m_point_lights[light_index].m_quadratic = new_quadratic;
	m_lights_buffer.fill(
		light_index * sizeof(PointLight) + offsetof(PointLight, m_quadratic),
		member_size(PointLight, m_quadratic),
		&new_quadratic);
}

float RenderSystem::getPointLightQuadratic(uint light_index)
{
	if (light_index >= m_point_lights.size())
	{
		LOG_ERROR_S("received light index out of range | size = [%d] | light_index = [%d]", m_point_lights.size(), light_index);
		return 0.0f;
	}
	return m_point_lights[light_index].m_quadratic;
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
