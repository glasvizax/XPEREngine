#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"

struct GameTimer
{
	float current_time;
	float dt;

	void update() 
	{
		float tmp = glfwGetTime();
		dt = tmp - current_time;
		current_time = tmp;
	}
};
