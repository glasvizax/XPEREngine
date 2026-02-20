#pragma once

#include <functional>
#include <map>
#include <string>

#include "xm/xm.h"

template <>
struct std::hash<std::pair<int, int>>
{
	size_t operator()(const std::pair<int, int>& p) const
	{
		return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
	};
};

struct GLFWwindow;

using KeyCallback = void		 (*)(void*);
using MouseButtonCallback = void (*)(void*);
using CursorPosCallback = void	 (*)(void*, double, double);

class InputManager
{
private:
	friend class Engine;

	bool init(GLFWwindow* window);

	void destroy();

public:
	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(InputManager&&) noexcept = delete;
	InputManager& operator=(InputManager&&) noexcept = delete;

	// action - GLFW_PRESS + GLFW_RELEASE + GLFW_REPEAT
	bool regKeyCallback(KeyCallback callback, int key, int modifier, int action);
	
	// action - GLFW_PRESS + GLFW_RELEASE
	bool regMouseCallback(KeyCallback callback, int button, int modifier, int action);

	bool regCursorPosCallback(CursorPosCallback callback);

	bool unregKeyCallback(int key, int modifier, int action);
	bool unregMouseCallback(int button, int modifier, int action);
	bool unregCursorPosCallback();

	xm::dvec2 getCursorPos() const;

	void setCustomPtr(void* ptr);

private:
	std::unordered_map<std::pair<int, int>, KeyCallback>		 m_key_callbacks[3];
	std::unordered_map<std::pair<int, int>, MouseButtonCallback> m_mouse_callbacks[2];
	CursorPosCallback											 m_cursor_pos_callback;

	bool m_has_cursor_pos_cb = false;

	xm::dvec2 m_cursor_pos;

private:
	static void _glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifier);
	static void _glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void _glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void _glfwMouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	void* m_custom_ptr = nullptr;
};
