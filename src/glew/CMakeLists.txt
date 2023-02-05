cmake_minimum_required(VERSION 3.0.0)
project(glew)
	include_directories(include)
	
	add_library(glew32s STATIC src/glew.c)