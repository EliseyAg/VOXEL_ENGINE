cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME VoxelEngine)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/Window/Window.hpp
	src/Window/Window.cpp

	src/voxel_engine.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)