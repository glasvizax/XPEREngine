#pragma once

#ifdef _DEBUG
	
	#include <string>
	#include <glad/glad.h>
	#include "Debug.h"

void __checkFramebufferErrorsGL(const char* file, int line, const std::string& culprit);

void __checkGeneralErrorGL(const char* file, int line, const std::string& culprit);

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam);


	#define checkFramebufferErrorsGL(__name__) __checkFramebufferErrorsGL(__FILE__, __LINE__, __name__)

	#define checkGeneralErrorGL(__name__) __checkGeneralErrorGL(__FILE__, __LINE__, __name__)

#else

	#define checkFramebufferErrorsGL() \
		do                             \
		{                              \
		}                              \
		while (0)

	#define checkGeneralErrorGL() \
		do                        \
		{                         \
		}                         \
		while (0)

#endif
