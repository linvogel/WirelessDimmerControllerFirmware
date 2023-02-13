set(TARGET "local" CACHE STRING "Select TARGET from <local|server|raspi>")
if(NOT TARGET)
message(FATAL_ERROR "TARGET was undefined! run cmake with -DTARGET=<local|server|raspi>")
endif()

if("${TARGET}" STREQUAL "local")
	list(APPEND LIBRARIES GLEW glfw opengl32)
		
	# because msvc is your friend, disable stupid warnings for code that needs to run on linux
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif("${TARGET}" STREQUAL "server")
	list(APPEND LIBRARIES GL GLU glut GLEW glfw)
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
	include_directories(src/glew/include glfw/include)
	
	if("${TARGET}" STREQUAL "local")
		add_definitions(-DGLEW_BUILD -DGLEW_STATIC)
		add_subdirectory(src/glew)
		remove_definitions(-DGLEW_BUILD)
		add_subdirectory(glfw)
	endif()
	
	foreach(SOURCE_FILE ${MAIN_SOURCES})
		get_filename_component(FNAME ${SOURCE_FILE} NAME_WE)
		remove_definitions(-D_DIMMER_FILE_NAME_)
		add_definitions(-D_DIMMER_FILE_NAME_="${FNAME}")
	endforeach()
	
	add_executable(firmware ${MAIN_SOURCES})
	target_link_libraries(firmware PUBLIC ${LIBRARIES})
	
	if("${TARGET}" STREQUAL "local")
		#target_link_directories(firmware PUBLIC $<TARGET_FILE_DIR:glfw> $<TARGET_FILE_DIR:GLEW>)
	endif()
