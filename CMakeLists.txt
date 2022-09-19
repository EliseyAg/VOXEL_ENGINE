cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME VoxelEngine)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/voxel_engine.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)