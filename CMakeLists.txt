cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME VoxelEngine)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/Window/Window.hpp
	src/Window/Window.cpp
	src/Window/Events.hpp
	src/Window/Events.cpp
	
	src/Camera/Camera.hpp
	src/Camera/Camera.cpp
	
	src/Graphics/ShaderProgram.hpp
	src/Graphics/ShaderProgram.cpp
	src/Graphics/Texture.hpp
	src/Graphics/Texture.cpp
	
	src/Loaders/png_loader.hpp
	src/Loaders/png_loader.cpp
	src/Loaders/stb_image.h

	src/voxel_engine.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(../Minecraft/external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
