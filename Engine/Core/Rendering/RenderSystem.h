#pragma once 

#include "ShaderProgram.h"
#include "VertexArray.h"
class RenderSystem
{
	friend class Engine;

	bool init();

	void render();

	void destroy();

	ShaderProgram test;
	VertexArray	  vao;
};
