find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

file(
	GLOB_RECURSE 
	ENGINE_SRC 
	LIST_DIRECTORIES true 
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h" 
)

foreach(_ext IN LISTS GLSL_EXTS)
  file(
	  GLOB_RECURSE 
	  TMP 
	  CONFIGURE_DEPENDS
	  "${CMAKE_CURRENT_SOURCE_DIR}/*.${_ext}"
  )
  list(APPEND GLSL_FILES ${TMP})
endforeach()

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR} 
	FILES ${ENGINE_SRC} ${GLSL_FILES}
)

file(
	GLOB_RECURSE 
	GLAD_FILES 
	"${GLAD_SOURCE_DIR}/*.c" 
	"${GLAD_SOURCE_DIR}/*.h" 
)

source_group(
	TREE ${GLAD_SOURCE_DIR} 
	PREFIX "ThirdParty/Glad" 
	FILES ${GLAD_FILES}
)

file(
	GLOB_RECURSE 
	STB_IMAGE_FILES 
	"${STB_IMAGE_SOURCE_DIR}/*.c" 
	"${STB_IMAGE_SOURCE_DIR}/*.h"
)

source_group(
	TREE ${STB_IMAGE_SOURCE_DIR} 
	PREFIX "ThirdParty/stb_image" 
	FILES ${STB_IMAGE_FILES}
)

set(ENGINE_LIB_SRC ${ENGINE_SRC} ${GLAD_FILES} ${STB_IMAGE_FILES})

list(REMOVE_ITEM ENGINE_LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_library(Engine SHARED ${ENGINE_LIB_SRC} ${GLSL_FILES})

foreach(i IN LISTS ENGINE_SRC)
	if(IS_DIRECTORY ${i})
		list(APPEND ENGINE_SUBDIRS ${i})
	endif()
endforeach()

foreach(subdir IN LISTS ENGINE_SUBDIRS)
  target_include_directories(Engine PRIVATE ${subdir})
endforeach()

target_link_libraries(Engine 
	PRIVATE glfw 
	PRIVATE opengl32.lib
	PRIVATE assimp::assimp
)

target_include_directories(
	Engine 
	PUBLIC "${GLAD_SOURCE_DIR}/include"
	PUBLIC "${STB_IMAGE_SOURCE_DIR}"
	PUBLIC "${GLM_SOURCE_DIR}"
	PUBLIC "${XM_SOURCE_DIR}"
	PUBLIC "${ASSIMP_INSTALL_DIR}/include"
)

add_executable(Main "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(Main PRIVATE Engine)

foreach(subdir IN LISTS ENGINE_SUBDIRS)
  target_include_directories(Main PRIVATE ${subdir})
endforeach()

add_custom_command(
	TARGET Engine
    POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLSL_FILES} "$<TARGET_FILE_DIR:Engine>"
	COMMENT "copying of glsl files..."
    VERBATIM
    COMMAND_EXPAND_LISTS
)

set(XPER_CONTENT_PATH "content" CACHE PATH "path to external content")

cmake_path(
	ABSOLUTE_PATH XPER_CONTENT_PATH 
	BASE_DIRECTORY ${CMAKE_SOURCE_DIR}
	NORMALIZE 
	OUTPUT_VARIABLE content_full_path
)

target_compile_definitions(
	Engine 
	PRIVATE XPER_CONTENT_PATH="${content_full_path}"
	PUBLIC GLFW_INCLUDE_NONE
)

#TODO : fix me!
set_target_properties(Main Engine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Main>)

