#pragma once

#include "Camera.h"
#include "GameTimer.h"

class PlayerInputManager
{

public:
	friend class Engine;

	PlayerInputManager() = default;

private:
	bool init();

	void update(float dt);

	void destroy();

	Camera* m_camera = nullptr;

	float m_speed = 3.0f;
	float m_sensitivity_vertical = 3.0f;
	float m_sensitivity_horizontal = 3.0f;

	bool m_forward;
	bool m_backward;
	bool m_left;
	bool m_right;

	static void moveForward(void* ptr);
	static void moveBackward(void* ptr);
	static void moveRight(void* ptr);
	static void moveLeft(void* ptr);

	static void moveForwardStop(void* ptr);
	static void moveBackwardStop(void* ptr);
	static void moveRightStop(void* ptr);
	static void moveLeftStop(void* ptr);

	static void cursorMoving(void* ptr, double xpos, double ypos);
	bool		m_fisrt_cursor_moving;
	float		m_xpos_last;
	float		m_ypos_last;
	float		m_xoffset;
	float		m_yoffset;
};