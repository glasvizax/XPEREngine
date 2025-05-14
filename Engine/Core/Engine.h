#pragma once

class ResourceManager;
class InputManager;
class RenderSystem;
class WindowManager;
struct GameTimer;

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
	GameTimer&		 getGameTimer();
};

extern "C"
{
	int runWinForms(void* panel_handle);
	int runStandalone();
};
