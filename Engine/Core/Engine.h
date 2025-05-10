#pragma once

class ResourceManager;
class InputManager;
class RenderSystem;
class WindowManager;

class Engine
{
	Engine() = default;

public:
	static Engine& getInstance();

	int runWinForms(void* panel_handle);
	int runStandalone();

	RenderSystem&	 getRenderSystem();
	WindowManager&	 getWindowManager();
	InputManager&	 getInputManager();
	ResourceManager& getResourceManager();
};

extern "C"
{
	int runWinForms(void* panel_handle);
	int runStandalone();
};
