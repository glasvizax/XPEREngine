cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0091 NEW)

project(XPEREngine VERSION 1.0.0 LANGUAGES CXX C)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	message(STATUS "MSVC COMPILER PASS")
endif()

set(ENGINE_SRC_PATH "Engine")
set(TEST_SRC_PATH "EngineTest")
set(EDITOR_SRC_PATH "Editor")
set(BIN_PATH "Binary")

set(GLSL_EXTS "vert;frag;geom")


set(GLFW3_INSTALL_DIR "" CACHE PATH "Path to glfw source directory")
set(GLAD_SOURCE_DIR "" CACHE PATH "Path to glad source directory")
set(STB_IMAGE_SOURCE_DIR "" CACHE PATH "Path to stb_image source directory")
set(GLM_SOURCE_DIR "" CACHE PATH "Path to glm source directory")
set(ASSIMP_INSTALL_DIR "" CACHE PATH "Path to directory with installed Assimp")
set(ASSIMP_VERSION "5.4" CACHE STRING "Assimp version")
set(GTEST_INSTALL_DIR "" CACHE PATH "Path to directory with installed GTest")

if(EXISTS "${CMAKE_SOURCE_DIR}/LocalPaths.cmake")
  include("${CMAKE_SOURCE_DIR}/LocalPaths.cmake")
endif()

foreach(var IN ITEMS 
	GLAD_SOURCE_DIR 
	STB_IMAGE_SOURCE_DIR 
	GLM_SOURCE_DIR 
	ASSIMP_INSTALL_DIR
	GLFW3_INSTALL_DIR
	ASSIMP_VERSION
	GTEST_INSTALL_DIR 
) 
	if(NOT DEFINED ${var})
		message(FATAL_ERROR "${var} is not defined")
	endif()
endforeach()	

set(CMAKE_PREFIX_PATH 
	"${CMAKE_PREFIX_PATH}" 
	"${GLFW3_INSTALL_DIR}/lib/cmake/glfw3" 
	"${ASSIMP_INSTALL_DIR}/lib/cmake/assimp-${ASSIMP_VERSION}" 
	"${GTEST_INSTALL_DIR}/lib/cmake/GTest"
)

add_subdirectory(${ENGINE_SRC_PATH})
add_subdirectory(${TEST_SRC_PATH})
add_subdirectory(${EDITOR_SRC_PATH})
