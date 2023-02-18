#include "window.hpp"
#include "../errors.hpp"

dim::gui::window::window(std::string name, int width, int height)
	: component(0.0, 0.0, 1.0, 1.0, 0.0)
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
		fatal("Unable to create GLFWwindow!");
		exit(dim::GLFW_WINDOW_CREATE_FAIL);
	}
	verbose("GLFWwindow constructed successfully.");
	
	verbose("Creating renderer object");
	this->m_renderer = renderer(this->m_window);
	
	// setup input handling
	this->m_input_ctrl = std::make_shared<dim::in::input_controller>(this->m_window, this);
	
	// setup colors
	// TODO: this should be read from the configuration
	this->m_bg_color = dim::math::vector4f({0.0f, 0.145f, 0.274f, 1.0f});
	this->m_fg_color = dim::math::vector4f({0.7f, 0.7f, 0.1f, 1.0f});
	
	this->m_shape = std::make_shared<quad2>(this->m_renderer, 0.0f, 0.0f, 800.0f, 0.0f, 800.0f, 480.0f, 0.0f, 480.0f);
	this->m_size = this->m_shape->get_size();
	this->m_shape->set_background_color(this->m_bg_color);
}

dim::gui::window::~window()
{
	debug("Destroying GLFW window...");
	glfwDestroyWindow(this->m_window);
	info("Terminating GLFW...");
	glfwTerminate();
}

void dim::gui::window::draw_component(dim::gui::renderer &renderer)
{
	if (this->m_shape.get()) renderer.draw_shape(this->m_shape.get());
}

int dim::gui::window::shoud_close()
{
	return glfwWindowShouldClose(this->m_window);
}

void dim::gui::window::handle_click_event(dim::event::click_event event)
{
	debug("Window received click event at (%f,%f)", event.get_location()(0), event.get_location()(1));
}