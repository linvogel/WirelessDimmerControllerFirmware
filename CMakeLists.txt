set(TARGET "local" CACHE STRING "Select TARGET from <local|server|raspi>")
if(NOT TARGET)
message(FATAL_ERROR "TARGET was undefined! run cmake with -DTARGET=<local|server|raspi>")
endif()

if("${TARGET}" STREQUAL "local")
list(APPEND LIBRARIES glew32 glfw opengl32)
elseif("${TARGET}" STREQUAL "server")
list(APPEND LIBRARIES GLEW glfw)
elseif("${TARGET}" STREQUAL "raspi")
list(APPEND LIBRARIES GL GLU glut GLEW glfw)
else()
message(FATAL_ERROR "Invalid target '${TARGET}'! run cmake with -DTARGET=<local|server|raspi>")
endif()


cmake_minimum_required(VERSION 3.0.0)
project(DimmerControllerFirmware)
	set(CMAKE_CXX_STANDARD 17)
	
	# find sources for main executable
	file(GLOB MAIN_SOURCES src/firmware/*.cpp)
	include_directories(src/glew/include)
	
	if("$(TARGET)" STREQUAL "local")
		set(BUILD_SHARED_LIBS ON)
		add_subdirectory(glfw)
		add_subdirectory(src/glew)
		add_definitions(-DGLFW_DLL)
	endif()
	
	foreach(SOURCE_FILE ${MAIN_SOURCES})
		get_filename_component(FNAME ${SOURCE_FILE} NAME_WE)
		add_definitions(-D_DIMMER_FILE_NAME_="${FNAME}")
	endforeach()
	
	add_executable(firmware ${MAIN_SOURCES})
	target_link_libraries(firmware PUBLIC ${LIBRARIES})
