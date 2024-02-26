cmake_minimum_required()

set(PROJECT_NAME VOXEL_ENGINE)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
)

set(PUBLIC_INCLUDES
	src/Events/Event.hpp
	
	src/Resources/ResourceManager.hpp
	src/Resources/stb_image.hpp

	src/Rendering/OpenGL/Window.hpp
	src/Rendering/OpenGL/Texture2D.hpp
	src/Rendering/OpenGL/ShaderProgram.hpp
	src/Rendering/OpenGL/IndexBuffer.hpp
	src/Rendering/OpenGL/VertexBuffer.hpp
	src/Rendering/OpenGL/VertexArray.hpp
	src/Rendering/OpenGL/VertexBufferLayout.hpp
	src/Rendering/OpenGL/Renderer.hpp
	src/Rendering/OpenGL/Camera.hpp
)

set(PUBLIC_SOURCES
	src/Resources/ResourceManager.cpp

	src/Rendering/OpenGL/Window.cpp
	src/Rendering/OpenGL/Texture2D.cpp
	src/Rendering/OpenGL/ShaderProgram.cpp
	src/Rendering/OpenGL/IndexBuffer.cpp
	src/Rendering/OpenGL/VertexBuffer.cpp
	src/Rendering/OpenGL/VertexArray.cpp
	src/Rendering/OpenGL/VertexBufferLayout.cpp
	src/Rendering/OpenGL/Renderer.cpp
	src/Rendering/OpenGL/Camera.cpp
)

set(ALL_SOURCES
	${PUBLIC_INCLUDES}
	${PUBLIC_SOURCES}
)

add_library(${PROJECT_NAME} STATIC
	${ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${ALL_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC source)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

include_directories(external/glm)
