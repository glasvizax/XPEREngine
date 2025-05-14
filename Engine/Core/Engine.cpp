#include "Engine.h"
#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WindowManager.h"
#include "InputManager.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "GameTimer.h"
#include "PlayerInputManager.h"
#include "Debug.h"

WindowManager g_window_manager;
InputManager  g_input_manager;
RenderSystem  g_render_system;
ResourceManager g_resource_manager;
PlayerInputManager g_player_input_manager;
GameTimer		g_game_timer;

Engine& Engine::getInstance()
{
	static Engine engine;
	return engine;
}

int Engine::runWinForms(void* panel_handle)
{
	if (!g_resource_manager.init())
	{
		return EXIT_FAILURE;
	}

	glm::value_ptr(glm::mat4(1.0f));

	if (!g_window_manager.initAsChild("test", panel_handle))
	{
		return EXIT_FAILURE;
	}
	if (!g_input_manager.init(g_window_manager.getWindow()))
	{
		return EXIT_FAILURE;
	}
	if (!g_render_system.init())
	{
		return EXIT_FAILURE;
	}

	while (!g_window_manager.isShouldClose())
	{
		g_window_manager.pollEvents();

		g_render_system.render();

		g_window_manager.swapBuffers();
	}

	g_render_system.destroy();
	g_input_manager.destroy();
	g_window_manager.destroy();
	g_resource_manager.destroy();

	return EXIT_SUCCESS;
}

int Engine::runStandalone()
{
	if (!g_resource_manager.init())
	{
		return EXIT_FAILURE;
	}
	if (!g_window_manager.init("test", 800, 600, false))
	{
		return EXIT_FAILURE;
	}
	if (!g_input_manager.init(g_window_manager.getWindow()))
	{
		return EXIT_FAILURE;
	}
	if (!g_render_system.init())
	{
		return EXIT_FAILURE;
	}
	if (!g_player_input_manager.init())
	{
		return EXIT_FAILURE;
	}
	
	g_input_manager.regKeyCallback([](void*) { g_window_manager.close(); }, GLFW_KEY_ESCAPE, NULL, GLFW_PRESS);

	while (!g_window_manager.isShouldClose())
	{
		g_window_manager.pollEvents();

		g_game_timer.update();

		g_player_input_manager.update(g_game_timer.dt);

		g_render_system.render();

		g_window_manager.swapBuffers();
	}

	g_render_system.destroy();
	g_input_manager.destroy();
	g_window_manager.destroy();
	g_resource_manager.destroy();

	return EXIT_SUCCESS;
}

RenderSystem& Engine::getRenderSystem()
{
	return g_render_system;
}

WindowManager& Engine::getWindowManager()
{
	return g_window_manager;
}

InputManager& Engine::getInputManager()
{
	return g_input_manager;
}

ResourceManager& Engine::getResourceManager()
{
	return g_resource_manager;
}

GameTimer& Engine::getGameTimer()
{
	return g_game_timer;
}

int runWinForms(void* panel_handle)
{
	Engine& engine = Engine::getInstance();
	return engine.runWinForms(panel_handle);
}

int runStandalone()
{
	Engine& engine = Engine::getInstance();
	return engine.runStandalone();
}
