
include(GoogleTest)
enable_testing() 

find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_FILES})

add_executable(Test ${TEST_FILES})

target_link_libraries(Test 
	PRIVATE GTest::gtest_main
	PRIVATE Engine
)

foreach(subdir IN LISTS ENGINE_SUBDIRS)
  target_include_directories(Test PRIVATE ${subdir})
endforeach()

gtest_discover_tests(Test)