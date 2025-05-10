#include "WindowManager.h"

#define GLFW_EXPOSE_NATIVE_WIN32

#include <iostream>

#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLFW/glfw3native.h"

#include "Debug.h"
#include "Constants.h"

using namespace std::string_literals;


bool WindowManager::init(const std::string& title, uint width, uint height, bool fullscreen)
{
	if (!glfwInit())
	{
		LOG_ERROR_S("couldn't init glfw: glfwInit() = 0");
		return false;
	}

	LOG_INFO_F("GLFW Version: %s", glfwGetVersionString());

	glfwSetErrorCallback([](int error_code, const char* description)
		{ 
			LOG_ERROR_F("GLFW Error: Error Code: %d, Description: %s", error_code, description);
		});

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef _DEBUG
	//TODO:
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

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
		LOG_ERROR_S("couldn't get primary monitor: glfwGetPrimaryMonitor() = nullptr");
		destroy();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwFocusWindow(m_window);
	glfwSwapInterval(1);

	glfwShowWindow(m_window);
	disableCursor();

	return true;
}

bool WindowManager::initAsChild(const std::string& title, void* parent_handle)
{
	if (!glfwInit())
	{
		LOG_ERROR_S("couldn't init glfw: glfwInit() = 0");
		return false;
	}

	LOG_INFO_F("GLFW Version: %s", glfwGetVersionString());

	glfwSetErrorCallback([](int error_code, const char* description) {
		LOG_ERROR_F("GLFW Error: Error Code: %d, Description: %s", error_code, description);
	});
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef _DEBUG
	// TODO:
	// glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	m_window = glfwCreateWindow(1, 1, title.c_str(), nullptr, nullptr);

	if (!m_window)
	{
		glfwTerminate();
		return false;
	}

	m_primary_monitor = glfwGetPrimaryMonitor();
	if (!m_primary_monitor)
	{
		LOG_ERROR_S("couldn't get primary monitor: glfwGetPrimaryMonitor() = nullptr");
		destroy();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwFocusWindow(m_window);
	glfwSwapInterval(1);

	if (parent_handle != nullptr)
	{
		HWND parent_hwnd = static_cast<HWND>(parent_handle);
		HWND current_hwnd = glfwGetWin32Window(m_window);
		::SetParent(current_hwnd, parent_hwnd);

		LONG style = GetWindowLongPtr(current_hwnd, GWL_STYLE);
		style = (style & ~WS_POPUP) | WS_CHILD;
		SetWindowLongPtr(current_hwnd, GWL_STYLE, style);

		RECT rect;
		GetClientRect(parent_hwnd, &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		MoveWindow(current_hwnd, 0, 0, w, h, TRUE);
	}
	glfwShowWindow(m_window);

	return true;
}

void WindowManager::setWindowPos(size_t x, size_t y)
{
	glfwSetWindowPos(m_window, x, y);
}

void WindowManager::swapBuffers()
{
	glfwMakeContextCurrent(m_window);
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

glm::ivec2 WindowManager::getWindowSize() const
{
	glm::ivec2 size;
	glfwGetWindowSize(m_window, &size.x, &size.y);
	return size;
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
