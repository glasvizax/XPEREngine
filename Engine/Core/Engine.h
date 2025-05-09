#pragma once

class Engine
{
public:

	int runWinForms(void* panel_handle);
	int runStandalone();
};

extern "C"
{
	int runWinForms(void* panel_handle);
	int runStandalone();
};
