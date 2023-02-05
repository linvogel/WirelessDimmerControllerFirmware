#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>


int main()
{
	if (!glfwInit()) {
		std::cout << "ERROR" << std::endl;
	}
	
	GLFWwindow *window = glfwCreateWindow(800, 600, "hello GLFW", nullptr, nullptr);
	
	glfwMakeContextCurrent(window);
	
	glfwSwapInterval(1);
	
	GLuint buf;
	glCreateBuffers(512, &buf);
	
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