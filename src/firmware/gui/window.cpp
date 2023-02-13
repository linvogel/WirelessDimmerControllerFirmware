#include "window.hpp"
#include "../errors.hpp"

dim::gui::window::window(std::string name, int width, int height)
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
	
	verbose("Making context current");
	glfwMakeContextCurrent(this->m_window);
	trace("Context made current");
	
	debug("Initializing GLEW...");
	if (glewInit() != GLEW_OK) {
		fatal("GLEW could not be initialized.");
		exit(dim::GLEW_INIT_FAIL);
	}
	verbose("GLEW initialized successfully!");
	
	// setup colors
	// TODO: this should be read from the configuration
	this->m_bg_color = dim::math::vector4f({0.0f, 0.0f, 0.3f, 0.7f});
	this->m_fg_color = dim::math::vector4f({0.0f, 0.0f, 0.7f, 1.0f});
	
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
	
}

int dim::gui::window::shoud_close()
{
	return glfwWindowShouldClose(this->m_window);
}