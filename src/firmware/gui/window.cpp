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
	this->m_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (!this->m_window) {
		fatal("Unable to create GLFWwindow!");
		exit(dim::GLFW_WINDOW_CREATE_FAIL);
	}
	verbose("GLFWwindow constructed successfully.");
	
	verbose("Creating renderer object");
	this->m_renderer = renderer(this->m_window);
	
	// setup colors
	// TODO: this should be read from the configuration
	this->m_bg_color = dim::math::vector4f({0.0f, 0.0f, 0.3f, 0.7f});
	this->m_fg_color = dim::math::vector4f({0.0f, 0.0f, 0.7f, 1.0f});
	
	this->m_shapes = std::vector<dim::gui::shape2*>();
	
	this->m_shapes.push_back(new dim::gui::triang2(this->m_renderer, {100, 100}, {100, 200}, {200, 100}));
	this->m_shapes.push_back(new dim::gui::quad2(this->m_renderer, {600, 100}, {700, 100}, {700, 200}, {600, 200}));
	this->m_shapes.push_back(new dim::gui::circle2(this->m_renderer, 400, 300, 50));
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
	
	renderer.draw_shape(this->m_shapes[0]);
	renderer.draw_shape(this->m_shapes[1]);
	renderer.draw_shape(this->m_shapes[2]);
}

int dim::gui::window::shoud_close()
{
	return glfwWindowShouldClose(this->m_window);
}