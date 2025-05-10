#include "RenderSystem.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Defines.h"
#include "Debug.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "DebugOpenGL.h"
#include "WindowManager.h"

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

	Engine& engine = Engine::getInstance();
	ResourceManager& rm = engine.getResourceManager();
	WindowManager& wm = engine.getWindowManager();

#ifdef _DEBUG
	vao = VertexArray("vao");
	test = ShaderProgram("test");
#endif // DEBUG

	if (!rm.loadShaderProgram("test.vert", "test.frag", test))
	{
		return false;
	}

	test.setVec("additive", glm::vec3(0.3f, 0.1f, 0.0f));
	vao.init();
	vao.bind();
	vao.attachBuffer(sizeof(vertices), vertices, BufferType::ARRAY);
	vao.enableAttribute(0, 2, 5, 0);
	vao.enableAttribute(1, 3, 5, 2);

	glm::ivec2 window_size = wm.getWindowSize();
	glViewport(0, 0, window_size.x, window_size.y);
	checkGeneralErrorGL("render_system");

	return true;
}

void RenderSystem::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	test.use();
	vao.bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RenderSystem::destroy()
{

}
