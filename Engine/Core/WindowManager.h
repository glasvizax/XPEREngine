#pragma once
#include <string>

#include "glm/glm.hpp"

#include "Defines.h"

class Engine;
struct GLFWwindow;
struct GLFWmonitor;

class WindowManager
{
	friend class Engine;
	bool init(const std::string& title, uint width, uint height, bool fullscreen);
	bool initAsChild(const std::string& title, void* parent_handle = nullptr);
	
	void destroy();

public:
	void pollEvents();

	void setWindowPos(size_t x, size_t y);

	void swapBuffers();

	void enableCursor();
	void disableCursor();

	void setVsync(bool vsync);

	void close();

	bool isShouldClose() const;

	glm::ivec2 getWindowSize() const;
	glm::uvec2 getFramebufferSize() const;

	glm::dvec2 getCursorPos();

private:
	GLFWwindow* getWindow();

	GLFWwindow*	 m_window = nullptr;
	GLFWmonitor* m_primary_monitor = nullptr;

	bool m_vsync = false;
};