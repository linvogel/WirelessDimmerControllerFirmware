set(target "win" CACHE STRING "Build target for the generated project. Possible is 'win','x64' and 'arm64'")
message("Generating project with target: ${target}")

if("${target}" STREQUAL "arm64")
	set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
	set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
	set(CMAKE_LIBRARY_PATH /workspace/arm-libs)
	set(CMAKE_INCLUDE_PATH /usr/arm-linux-gnueabihf/include;/usr/include)
	
	include_directories(BEFORE /usr/include /usr/arm-linux-gnueabihf/include)
	
	set(CMAKE_SYSTEM_NAME Linux)
elseif("${target}" STREQUAL "x64")
	set(CMAKE_C_COMPILER gcc)
	set(CMAKE_CXX_COMPILER g++)

	set(CMAKE_SYSTEM_NAME Linux)
elseif("${target}" STREQUAL "win")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	
	set(CMAKE_SYSTEM_NAME Windows)
else()
	message( FATAL_ERROR "invalid target '${target}'! Known targets are 'win', 'x64', 'arm64'.")
endif()

cmake_minimum_required(VERSION 3.0.0)
project(DimmerControllerFirmware)
	if("${target}" STREQUAL "win")
		set(CMAKE_CXX_STANDARD 23)
	else()
		set(CMAKE_CXX_STANDARD 20)
	endif()
	
	# find sources for main executable
	file(GLOB MAIN_SOURCES src/firmware/*.cpp)
	include_directories(src/glew/include)
	
	if("${target}" STREQUAL "win")
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

	if("${target}" STREQUAL "arm64")
		target_link_directories(firmware PUBLIC "./arm-libs/")
		#	target_link_libraries(firmware PUBLIC X11)
	endif()

	#target_link_libraries(firmware PUBLIC GLEW glfw)
	
	if("${target}" STREQUAL "win")
		add_custom_command(TARGET firmware POST_BUILD
			COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:glew32> $<TARGET_FILE_DIR:firmware>
			COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:firmware>
		)
	endif()
