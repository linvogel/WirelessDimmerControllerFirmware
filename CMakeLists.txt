cmake_minimum_required(VERSION 3.0.0)
project(DimmerControllerFirmware)
	set(CMAKE_CXX_STANDARD 20)
	# find sources for main executable
	file(GLOB MAIN_SOURCES src/firmware/*.cpp)
	
	add_executable(firmware ${MAIN_SOURCES})

find_package(Boost REQUIRED)
target_include_directories(firmware PUBLIC ${Boost_INCLUDE_DIRS})