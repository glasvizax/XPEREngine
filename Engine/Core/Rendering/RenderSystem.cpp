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

#ifdef _DEBUG

	test_tex = Texture("test_tex");
	vao = VertexArray("vao");
	test = ShaderProgram("test");

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

	if (!rm.initLoadShaderProgram("test.vert", "test.frag", test))
	{
		return false;
	}
	if (!rm.initLoadTexture("content/wood.jpg", test_tex, true))
	{
		//TODO
	}

	test.set("tex", 0);
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
	test_tex.bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RenderSystem::destroy()
{

}
