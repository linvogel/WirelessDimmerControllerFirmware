#include "window.hpp"
#include "../errors.hpp"

#define MODULE_NAME "window"
#include "../logging.hpp"

using namespace dim::gui;


window::window(std::string name, int width, int height)
	: component(0.0, 0.0, 1.0, 1.0, 0.0, (float)width, (float)height)
{
	info("Creating window...");
	debug("Initializing GLFW...");
	if (!glfwInit()) {
		fatal("GLFW could not be initialized.");
		exit(dim::GLFW_INIT_FAIL);
	}
	verbose("GLFW initialized successfully!");
	
	debug("Constructing GLFWwindow...");
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	this->m_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (!this->m_window) {
		const char* err_str = "N/A";
		glfwGetError(&err_str);
		fatal("Unable to create GLFWwindow: %s", err_str);
		exit(dim::GLFW_WINDOW_CREATE_FAIL);
	}
	verbose("GLFWwindow constructed successfully.");
	
	verbose("Creating renderer object");
	this->m_renderer = std::make_shared<renderer>(this->m_window);
	
	// setup input handling
	this->m_input_ctrl = std::make_shared<dim::in::input_controller>(this->m_window, this);
	
	// setup colors
	// TODO: this should be read from the configuration
	
	this->m_shape = std::make_shared<quad2>(*(this->m_renderer), 0.0f, 0.0f, 800.0f, 0.0f, 800.0f, 480.0f, 0.0f, 480.0f);
	this->m_shape->set_background_color(this->m_bg_color);
		
	this->m_size = this->m_shape->get_size();
}

window::~window()
{
	debug("Destroying GLFW window...");
	glfwDestroyWindow(this->m_window);
	info("Terminating GLFW...");
	glfwTerminate();
}

void window::draw_component(renderer &renderer)
{
	if (this->m_shape.get()) renderer.draw_shape(this->m_shape.get());
}

int window::shoud_close()
{
	return glfwWindowShouldClose(this->m_window);
}