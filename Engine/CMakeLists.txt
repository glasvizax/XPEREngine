find_package(glfw3 CONFIG REQUIRED)
find_package(assimp REQUIRED)

# getting engine src files
file(GLOB_RECURSE ENGINE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h" 
)

set(GLSL_FILES)
foreach(_ext IN LISTS GLSL_EXTS)
  file(GLOB_RECURSE TMP "${CMAKE_CURRENT_SOURCE_DIR}/*.${_ext}")
  list(APPEND GLSL_FILES ${TMP})
endforeach()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SRC} ${GLSL_FILES})

# getting glad src files
file(GLOB_RECURSE GLAD_FILES 
	"${GLAD_SOURCE_DIR}/*.c" 
	"${GLAD_SOURCE_DIR}/*.h" 
)

source_group(TREE ${GLAD_SOURCE_DIR} PREFIX "ThirdParty/Glad" FILES ${GLAD_FILES})
# getting stb image src files
file(GLOB_RECURSE STB_IMAGE_FILES 
	"${STB_IMAGE_SOURCE_DIR}/*.c" 
	"${STB_IMAGE_SOURCE_DIR}/*.h"
)

source_group(TREE ${STB_IMAGE_SOURCE_DIR} PREFIX "ThirdParty/stb_image" FILES ${STB_IMAGE_FILES})

set(ENGINE_LIB_SRC ${ENGINE_SRC} ${GLAD_FILES} ${STB_IMAGE_FILES})

list(REMOVE_ITEM ENGINE_LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_library(Engine SHARED ${ENGINE_LIB_SRC} ${GLSL_FILES})

# get all subdirs of ENGINE_SRC_PATH
file(GLOB_RECURSE ENGINE_SUBDIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*")
list(FILTER ENGINE_SUBDIRS INCLUDE REGEX ".*/[^/]+$")

foreach(subdir IN LISTS ENGINE_SUBDIRS)
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

add_executable(Main "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

target_link_libraries(Main PRIVATE Engine)

foreach(subdir IN LISTS ENGINE_SUBDIRS)
  target_include_directories(Main PRIVATE ${subdir})
endforeach()

add_custom_command(
    TARGET Engine
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLSL_FILES} ${BIN_PATH}
)
