#include <iostream>
#include "Engine.h"

int main() 
{
	Engine& engine = Engine::getInstance();
	return engine.runStandalone();

}