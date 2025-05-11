#pragma once

#include "Camera.h"

class PlayerController
{
	friend class Engine;
	bool init();

	void destroy();

	Camera m_camera;
};