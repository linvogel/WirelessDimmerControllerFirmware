cmake_minimum_required(VERSION 3.0.0)
project(glew)
	include_directories(include)
	add_definitions(-DGLEW_STATIC)
	
	add_library(glew32s STATIC src/glew.c)