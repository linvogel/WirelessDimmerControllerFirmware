#include "renderer.hpp"

dim::gui::renderer::renderer(GLFWwindow *window)
{
	this->m_window = window;
}

dim::gui::renderer::~renderer()
{
	
}

void dim::gui::renderer::wait()
{
	glfwWaitEvents();
}

void dim::gui::renderer::wait(double timeout)
{
	glfwWaitEventsTimeout(timeout);
}

void dim::gui::renderer::poll()
{
	glfwPollEvents();
}

void dim::gui::renderer::swap()
{
	glfwSwapBuffers(this->m_window);
}

void dim::gui::renderer::set_swap_interval(int i)
{
	glfwSwapInterval(i);
}