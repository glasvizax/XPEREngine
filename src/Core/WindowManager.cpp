#include "WindowManager.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Debug.h"
#include "Constants.h"

using namespace std::string_literals;

bool WindowManager::init(const std::string& title, uint width, uint height, bool fullscreen)
{
	if (!glfwInit())
	{
		LOG_ERROR("couldn't init glfw: glfwInit() = 0");
		return false;
	}

	LOG_INFO("GLFW Version: "s + glfwGetVersionString());

	glfwSetErrorCallback([](int error_code, const char* description)
		{ 
			LOG_ERROR("GLFW Error: Error Code: "s + std::to_string(error_code) + ", Description: " + description);
		});
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//glfwWindowHint(GLFW_SAMPLES, 4); - due to large amount of g buffer i disabled it

#ifdef _DEBUG
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	if (fullscreen)
	{
		m_window = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}
	else
	{
		m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	}

	if (!m_window)
	{
		glfwTerminate();
		return false;
	}

	m_primary_monitor = glfwGetPrimaryMonitor();
	if (!m_primary_monitor)
	{
		LOG_ERROR("couldn't get primary monitor: glfwGetPrimaryMonitor() = nullptr");
		destroy();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwFocusWindow(m_window);
	glfwSwapInterval(1);

	disableCursor();

	return true;
}

void WindowManager::setWindowPos(size_t x, size_t y)
{
	glfwSetWindowPos(m_window, x, y);
}

void WindowManager::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

void WindowManager::enableCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void WindowManager::disableCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowManager::setVsync(bool vsync) 
{
	m_vsync = vsync;
	glfwSwapInterval(static_cast<int>(vsync));
}

void WindowManager::close()
{
	glfwSetWindowShouldClose(m_window, true);
}

bool WindowManager::isShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

glm::uvec2 WindowManager::getWindowSize() const
{
	const GLFWvidmode* video_mode = glfwGetVideoMode(m_primary_monitor);
	return { video_mode->width, video_mode->height };
}

glm::uvec2 WindowManager::getFramebufferSize() const
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	return { width, height };
}

glm::dvec2 WindowManager::getCursorPos()
{
	glm::dvec2 pos;
	glfwGetCursorPos(m_window, &pos.x, &pos.y);
	return pos;
}

void WindowManager::pollEvents()
{
	glfwPollEvents();
}

void WindowManager::destroy()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

GLFWwindow* WindowManager::getWindow()
{
	return m_window;
}
