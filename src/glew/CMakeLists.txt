cmake_minimum_required(VERSION 3.0.0)
project(glew)
	add_definitions(-DGLEW_BUILD -DGLEW_STATIC)
	include_directories(include)
	
	add_library(GLEW STATIC src/glew.c)
	target_link_libraries(GLEW PRIVATE opengl32 gdi32)
