cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.10)
include(GoogleTest)
enable_testing() 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(XPEREngine)

set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(BIN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(GLSL_EXTS "vert;frag;geom")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH}")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

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

find_package(glfw3 CONFIG REQUIRED)
find_package(assimp REQUIRED)
find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE ENGINE_CPP_FILES
	"${SRC_PATH}/*.cpp" 
	"${SRC_PATH}/*.h" 
)

set(GLSL_FILES)
foreach(ext IN LISTS GLSL_EXTS)
  file(GLOB_RECURSE TMP "${SRC_PATH}/*.${ext}")
  list(APPEND GLSL_FILES ${TMP})
endforeach()

source_group(TREE ${SRC_PATH} PREFIX "Engine" FILES ${ENGINE_CPP_FILES} ${GLSL_FILES})

file(GLOB_RECURSE GLAD_FILES 
	"${GLAD_SOURCE_DIR}/*.c" 
	"${GLAD_SOURCE_DIR}/*.h" 
)

source_group(TREE ${GLAD_SOURCE_DIR} PREFIX "Glad" FILES ${GLAD_FILES})

file(GLOB_RECURSE STB_IMAGE_FILES 
	"${STB_IMAGE_SOURCE_DIR}/*.c" 
	"${STB_IMAGE_SOURCE_DIR}/*.h"
)

source_group(TREE ${STB_IMAGE_SOURCE_DIR} PREFIX "stb_image" FILES ${STB_IMAGE_FILES})

set(LIB_CPP ${ENGINE_CPP_FILES} ${GLAD_FILES} ${STB_IMAGE_FILES})

list(REMOVE_ITEM LIB_CPP "${SRC_PATH}/main.cpp")

add_library(Engine STATIC ${LIB_CPP} ${GLSL_FILES})

# Получить все поддиректории в src
file(GLOB_RECURSE SUBDIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/src/*")
list(FILTER SUBDIRS INCLUDE REGEX ".*/[^/]+$")

# Добавить их в include-директории
foreach(subdir IN LISTS SUBDIRS)
  target_include_directories(Engine PRIVATE ${subdir})
endforeach()

target_link_libraries(Engine 
	PRIVATE glfw 
	PRIVATE opengl32.lib
	PRIVATE assimp::assimp
)

target_include_directories(Engine 
	PUBLIC "${GLAD_SOURCE_DIR}/include"
	PUBLIC ${STB_IMAGE_SOURCE_DIR}
	PUBLIC ${GLM_SOURCE_DIR}
	PUBLIC "${ASSIMP_INSTALL_DIR}/include"
)

add_executable(Main "${SRC_PATH}/main.cpp")

target_link_libraries(Main PRIVATE Engine)

foreach(subdir IN LISTS SUBDIRS)
  target_include_directories(Main PRIVATE ${subdir})
endforeach()

file(GLOB_RECURSE TEST_FILES "${TEST_PATH}/*.cpp" "${TEST_PATH}/*.h")
source_group(TREE "${TEST_PATH}" FILES ${TEST_FILES})

add_executable(Test ${TEST_FILES})

target_link_libraries(Test 
	PRIVATE GTest::gtest_main
	PRIVATE Engine
)

target_include_directories(Test 
	PRIVATE ${SRC_PATH}
)

foreach(subdir IN LISTS SUBDIRS)
  target_include_directories(Test PRIVATE ${subdir})
endforeach()

gtest_discover_tests(Test)

add_custom_command(
    TARGET Main
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLSL_FILES} ${BIN_PATH}
)
