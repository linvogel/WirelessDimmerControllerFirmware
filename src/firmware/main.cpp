#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logging.hpp"

#include "math/matrix.hpp"

#ifndef SOMEVAR
#define SOMEVAR "OI"
#endif

using namespace dimmer::math;


int main()
{
	dimmer::logging::init();
	info("Starting DimmerControllerFirmware...");
	
	matrix2f id = {1, 0, 0, 1};
	matrix2f other = {1, 2, 3, 4};
	
	matrix<float,3,5> left = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2};
	matrix3f right = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	
	other += id;
	matrix2f m = id + other;
	matrix<float,3,5> prod = left * right;
	
	for (int i = 0; i < id.size(); i++) std::cout << id(i) << " "; std::cout << std::endl;
	for (int i = 0; i < other.size(); i++) std::cout << other(i) << " "; std::cout << std::endl;
	for (int i = 0; i < m.size(); i++) std::cout << m(i) << " "; std::cout << std::endl;
	for (int i = 0; i < left.size(); i++) std::cout << left(i) << " "; std::cout << std::endl;
	left *= right;
	for (int i = 0; i < prod.size(); i++) std::cout << prod(i) << " "; std::cout << std::endl;
	for (int i = 0; i < left.size(); i++) std::cout << left(i) << " "; std::cout << std::endl;
	
	info("Initializing GLFW...");
	if (!glfwInit()) {
		fatal("Unable to initialize GLFW!");
		return 1;
	}
	
	GLFWwindow *window = glfwCreateWindow(800, 480, "hello GLFW", nullptr, nullptr);
	
	glfwMakeContextCurrent(window);
	
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
}