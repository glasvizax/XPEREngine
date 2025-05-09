#include "InputManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Defines.h"
#include "Debug.h"
#include "Utils.h"

// clang-format off
constexpr int MODIFIER_BITS = 
		GLFW_MOD_SHIFT		| 
		GLFW_MOD_CONTROL	| 
		GLFW_MOD_ALT		|
		GLFW_MOD_SUPER		|
		GLFW_MOD_CAPS_LOCK	| 
		GLFW_MOD_NUM_LOCK;
// clang-format on

inline bool isModifier(int modifier)
{
	return (modifier & ~MODIFIER_BITS) == 0;
}

inline bool checkRegKeyCbHelper(int key, int modifier, int action)
{
	if (!isInRange(GLFW_KEY_SPACE, GLFW_KEY_LAST, key) || (action != NULL && action != GLFW_PRESS && action != GLFW_RELEASE && action != GLFW_REPEAT) || !isModifier(modifier))
	{
		return false;
	}
	return true;
}

inline bool checkRegMouseCbHelper(int button, int modifier, int action)
{
	if (!isInRange(GLFW_KEY_SPACE, GLFW_KEY_LAST, button) || (action != NULL && action != GLFW_PRESS && action != GLFW_RELEASE) || !isModifier(modifier))
	{
		return false;
	}
	return true;
}

bool InputManager::init(GLFWwindow* window)
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, InputManager::_glfwKeyCallback);

	glfwSetCursorPosCallback(window, InputManager::_glfwCursorPosCallback);
	glfwSetMouseButtonCallback(window, InputManager::_glfwMouseButtonCallback);

	// TODO
	// glfwSetWindowSizeCallback(window, InputManager::glfwWindowSizeCallback);

	return true;
}

void InputManager::destroy()
{

}


bool InputManager::regKeyCallback(Callback callback, int key, int modifier, int action)
{
	if (!checkRegKeyCbHelper(key, modifier, action))		
	{
		return false;
	}

	m_key_callbacks[action][std::make_pair(key, modifier)] = callback;
	return true;
}

bool InputManager::regMouseCallback(Callback callback, int button, int modifier, int action)
{
	if (!checkRegMouseCbHelper(button, modifier, action))	
	{
		return false;
	}

	m_mouse_callbacks[action][std::make_pair(button, modifier)] = callback;
	return true;
}

bool InputManager::unregKeyCallback(int key, int modifier, int action)
{
	if (!checkRegKeyCbHelper(key, modifier, action))		
	{
		return false;
	}

	return m_key_callbacks[action].erase(std::make_pair(key, modifier));
}

bool InputManager::unregMouseCallback(int button, int modifier, int action)
{
	if (!checkRegMouseCbHelper(button, modifier, action))	
	{
		return false;
	}

	return m_mouse_callbacks[action].erase(std::make_pair(button, modifier));
}

glm::dvec2 InputManager::getCursorPos() const
{
	return m_cursor_pos;
}

void InputManager::_glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifier)
{
	InputManager* self = scast<InputManager*>(glfwGetWindowUserPointer(window));
	auto&		  callbacks_map = self->m_key_callbacks[action];
	auto		  it = callbacks_map.find(std::make_pair(key, modifier));
	if (it != callbacks_map.end())
	{
		it->second();
	}
}

void InputManager::_glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputManager* self = scast<InputManager*>(glfwGetWindowUserPointer(window));
	self->m_cursor_pos = glm::dvec2(xpos, ypos);
}

void InputManager::_glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int modifier)
{
	InputManager* self = scast<InputManager*>(glfwGetWindowUserPointer(window));
	auto&		  callbacks_map = self->m_mouse_callbacks[action];
	auto		  it = callbacks_map.find(std::make_pair(button, modifier));
	if (it != callbacks_map.end())
	{
		it->second();
	}
}