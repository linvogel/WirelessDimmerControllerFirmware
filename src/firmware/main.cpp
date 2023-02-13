#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logging.hpp"

#include "math/matrix.hpp"

#include "errors.hpp"

#ifndef SOMEVAR
#define SOMEVAR "OI"
#endif

using namespace dimmer::math;


int main()
{
	dimmer::logging::init();
	info("Starting DimmerControllerFirmware...");
	
	info("Initializing GLFW...");
	if (glfwInit() != GLFW_TRUE) {
		fatal("Unable to initialize GLFW!");
		char *glfw_error;
		glfwGetError((const char**)&glfw_error);
		if (glfw_error) fatal("GLFW ERROR: %s", glfw_error);
		return dim::GLFW_INIT_FAIL;
	}
	debug("GLFW Initialized sucessfully!");
	
	GLFWwindow *window = glfwCreateWindow(800, 480, "hello GLFW", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	info("Initializing GLEW...");
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fatal("Unable to initialize GLEW!");
		fatal("GLEW ERROR: %s", glewGetErrorString(err));
		return dim::GLEW_INIT_FAIL;
	}
	debug("GLEW initialized sucessully!");
	
	
	
	glfwSwapInterval(1);
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBegin(GL_TRIANGLES);
			glColor3f(1, 0, 0);
			glVertex2f(0, -0.5f);
			glColor3f(0, 1, 0);
			glVertex2f(-0.5f, 0.5f);
			glColor3f(0, 0, 1);
			glVertex2f(0.5f, 0.5f);
		glEnd();
		
		glfwSwapBuffers(window);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	std::cout << "hello, world" << std::endl;
	
	return dim::OK;
}