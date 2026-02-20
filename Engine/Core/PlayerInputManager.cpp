#include "PlayerInputManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine.h"
#include "RenderSystem.h"
#include "InputManager.h"
#include "Camera.h"

#include "xm/xm.h"

bool PlayerInputManager::init(GLFWwindow* window)
{
	Engine& engine = Engine::getInstance();

	m_camera = &engine.getRenderSystem().getCamera();

	InputManager& input_manager = engine.getInputManager();

	m_render_system = &engine.getRenderSystem();

	input_manager.regCursorPosCallback(PlayerInputManager::cursorMoving);

	input_manager.regKeyCallback(PlayerInputManager::testK, GLFW_KEY_K, NULL, GLFW_PRESS);
	input_manager.regKeyCallback(PlayerInputManager::testH, GLFW_KEY_H, NULL, GLFW_PRESS);

	input_manager.setCustomPtr(this);

	m_window = window;
	//glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);

	return true;
}

void PlayerInputManager::update(float dt)
{ 
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		xm::vec3 move_dir = m_camera->getLookVector();
		xm::vec3 move = move_dir * m_speed * dt;
		m_camera->move(move);
	}

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		xm::vec3 move_dir = -m_camera->getLookVector();
		xm::vec3 move = move_dir * m_speed * dt;
		m_camera->move(move);
	}

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		xm::vec3 move_dir = m_camera->getRightVector();
		xm::vec3 move = move_dir * m_speed * dt;
		m_camera->move(move);
	}

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		xm::vec3 move_dir = -m_camera->getRightVector();
		xm::vec3 move = move_dir * m_speed * dt;
		m_camera->move(move);
	}

	if (m_xoffset)
	{
		float y = m_xoffset * m_sensitivity_vertical * dt;
		m_camera->rotateY(y);
		m_xoffset = 0.0f;
	}

	if (m_yoffset)
	{
		float x = m_yoffset * m_sensitivity_horizontal * dt;
		m_camera->rotateX(-x);
		m_yoffset = 0.0f;
	}
}

void PlayerInputManager::destroy()
{
	Engine& engine = Engine::getInstance();

	InputManager& input_manager = engine.getInputManager();

	input_manager.unregKeyCallback(GLFW_KEY_W, NULL, GLFW_PRESS);
	input_manager.unregKeyCallback(GLFW_KEY_S, NULL, GLFW_PRESS);
	input_manager.unregKeyCallback(GLFW_KEY_D, NULL, GLFW_PRESS);
	input_manager.unregKeyCallback(GLFW_KEY_A, NULL, GLFW_PRESS);

	input_manager.unregKeyCallback(GLFW_KEY_W, NULL, GLFW_RELEASE);
	input_manager.unregKeyCallback(GLFW_KEY_S, NULL, GLFW_RELEASE);
	input_manager.unregKeyCallback(GLFW_KEY_D, NULL, GLFW_RELEASE);
	input_manager.unregKeyCallback(GLFW_KEY_A, NULL, GLFW_RELEASE);

	input_manager.unregKeyCallback(GLFW_KEY_K, NULL, GLFW_PRESS);
	input_manager.unregKeyCallback(GLFW_KEY_H, NULL, GLFW_PRESS);

	input_manager.unregCursorPosCallback();
}

void PlayerInputManager::moveForward(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	self->m_forward = true;
}

void PlayerInputManager::moveBackward(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	self->m_backward = true;
}

void PlayerInputManager::moveRight(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	self->m_right = true;
}

void PlayerInputManager::moveLeft(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	self->m_left = true;
}

void PlayerInputManager::moveForwardStop(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	self->m_forward = false;
}

void PlayerInputManager::moveBackwardStop(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	self->m_backward = false;
}

void PlayerInputManager::moveRightStop(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	self->m_right = false;
}

void PlayerInputManager::moveLeftStop(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	self->m_left = false;
}

void PlayerInputManager::testK(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);
	self->m_render_system->testInputK();
}

void PlayerInputManager::testH(void* ptr)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);
	self->m_render_system->testInputH();
}

void PlayerInputManager::cursorMoving(void* ptr, double xpos, double ypos)
{
	PlayerInputManager* self = static_cast<PlayerInputManager*>(ptr);

	if (self->m_fisrt_cursor_moving)
	{
		self->m_xoffset = 0.0f;
		self->m_yoffset = 0.0f;
		self->m_fisrt_cursor_moving = false;
	}
	else
	{
		self->m_xoffset = self->m_xpos_last - xpos;
		self->m_yoffset = ypos - self->m_ypos_last;
	}
	self->m_xpos_last = xpos;
	self->m_ypos_last = ypos;
}
