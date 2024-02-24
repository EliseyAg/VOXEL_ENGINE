cmake_minimum_required()

set(PROJECT_NAME VOXEL_ENGINE)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
)

set(PUBLIC_INCLUDES
	
)
set(PUBLIC_SOURCES
	
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
