cmake_minimum_required(VERSION 3.0.0)
project(DimmerControllerFirmware)
	set(CMAKE_CXX_STANDARD 23)
	
	
	# find sources for main executable
	file(GLOB MAIN_SOURCES src/firmware/*.cpp)
	include_directories(src/glew/include)
	
	add_subdirectory(glfw)
	add_subdirectory(src/glew)
	foreach(SOURCE_FILE ${MAIN_SOURCES})
		get_filename_component(FNAME ${SOURCE_FILE} NAME_WE)
		add_definitions(-D_DIMMER_FILE_NAME_="${FNAME}")
	endforeach()
	
	add_executable(firmware ${MAIN_SOURCES})
	target_link_libraries(firmware PUBLIC glew32s glfw opengl32)