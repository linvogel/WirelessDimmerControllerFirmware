cmake_minimum_required(VERSION 3.0.0)
project(glew)
	add_definitions(-DGLEW_BUILD)
	include_directories(include)
	
	add_library(glew32 SHARED src/glew.c)
	target_link_libraries(glew32 PUBLIC opengl32 gdi32)