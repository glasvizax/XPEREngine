#include "Engine.h"
#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WindowManager.h"
#include "InputManager.h"

WindowManager g_window_manager;
InputManager  g_input_manager;

int Engine::runWinForms(void* panel_handle)
{
	if (!g_window_manager.initAsChild("test", panel_handle))
	{
		return EXIT_FAILURE;
	}
	if (!g_input_manager.init(g_window_manager.getWindow()))
	{
		return EXIT_FAILURE;
	}

	//g_input_manager.regKeyCallback([]() { g_window_manager.close(); }, GLFW_KEY_ESCAPE, NULL, GLFW_PRESS);

	while (!g_window_manager.isShouldClose())
	{
		g_window_manager.pollEvents();

		g_window_manager.swapBuffers();
	}

	g_window_manager.destroy();
	g_input_manager.destroy();

	return EXIT_SUCCESS;
}

int Engine::runStandalone()
{
	if (!g_window_manager.init("test", 800, 600, false))
	{
		return EXIT_FAILURE;
	}
	if (!g_input_manager.init(g_window_manager.getWindow()))
	{	
		return EXIT_FAILURE;
	}

	g_input_manager.regKeyCallback([]() { g_window_manager.close(); }, GLFW_KEY_ESCAPE, NULL, GLFW_PRESS);

	while (!g_window_manager.isShouldClose())
	{
		g_window_manager.pollEvents();


		g_window_manager.swapBuffers();
	}

	g_window_manager.destroy();
	g_input_manager.destroy();

	return EXIT_SUCCESS;
}

int runWinForms(void* panel_handle)
{
	Engine engine;
	return engine.runWinForms(panel_handle);
}

int runStandalone()
{
	Engine engine;
	return engine.runStandalone();
}
