#pragma once

#include <functional>
#include <map>
#include <string>

#include <glm/glm.hpp>

template <>
struct std::hash<std::pair<int, int>>
{
	std::size_t operator()(const std::pair<int, int>& p) const
	{
		return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
	};
};


struct GLFWwindow;

using Callback = std::function<void()>;

class InputManager
{
private:
	friend class Engine;

	bool init(GLFWwindow* window);

	void destroy();

public:
	bool regKeyCallback(Callback callback, int key, int modifier, int action);
	bool regMouseCallback(Callback callback, int button, int modifier, int action);

	bool unregKeyCallback(int key, int modifier, int action);
	bool unregMouseCallback(int button, int modifier, int action);


	glm::dvec2 getCursorPos() const;

private:
	std::unordered_map<std::pair<int, int>, Callback> m_key_callbacks[3];
	std::unordered_map<std::pair<int, int>, Callback> m_mouse_callbacks[2];

	glm::dvec2 m_cursor_pos;

private:
	static void _glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifier);
	static void _glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void _glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

};
