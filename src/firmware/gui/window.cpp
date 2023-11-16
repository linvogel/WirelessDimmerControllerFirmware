#include "window.hpp"
#include "../errors.hpp"

#define MODULE_NAME "window"
#include "../logging.hpp"

using namespace dim::gui;


window::window(std::string name, int width, int height)
	: component(0.0, 0.0, 1.0, 1.0, 0.0, (float)width, (float)height)
{
	ftrace();
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
	
	// setup scene 0
	this->m_scenes.push_back(std::vector<dim::gui::component*>());
	this->m_scene = 0;
	
	this->m_shape = std::make_shared<quad2>(*(this->m_renderer), 0.0f, 0.0f, 800.0f, 0.0f, 800.0f, 480.0f, 0.0f, 480.0f);
	this->m_shape->set_background_color(this->m_bg_color);
		
	this->m_size = this->m_shape->get_size();
	
	// setup uninitialized pointers
	this->m_onscreen_kbd = nullptr;
	this->m_channel_screen = nullptr;
}

window::~window()
{
	ftrace();
	debug("Destroying GLFW window...");
	glfwDestroyWindow(this->m_window);
	info("Terminating GLFW...");
	glfwTerminate();
}

void window::draw_component(renderer &renderer)
{
	ftrace();
	if (this->m_shape.get()) renderer.draw_shape(this->m_shape.get());
}

int window::shoud_close()
{
	ftrace();
	return glfwWindowShouldClose(this->m_window);
}

void window::add(dim::gui::component *child)
{
	ftrace();
	this->dim::gui::component::add(child);
	this->m_scenes[this->m_scene].push_back(child);
}

void window::remove_child(dim::gui::component *child)
{
	ftrace();
	this->dim::gui::component::remove_child(child);
	for (int64_t i = this->m_scenes[this->m_scene].size()-1; i >= 0; i--) {
		if (this->m_scenes[this->m_scene][i] == child) {
			this->m_scenes[this->m_scene].erase(this->m_scenes[this->m_scene].begin()+i);
		}
	}
}

void window::set_scene(size_t scene)
{
	ftrace();
	this->m_focussed = nullptr;
	this->m_children.clear();
	this->m_scene = scene;
	this->m_children.insert(this->m_children.begin(), this->m_scenes[this->m_scene].begin(), this->m_scenes[this->m_scene].end());
}

size_t window::create_scene()
{
	ftrace();
	size_t out = this->m_scenes.size();
	this->m_scenes.push_back(std::vector<dim::gui::component*>());
	return out;
}

void window::push_scene(size_t scene)
{
	ftrace();
	this->m_scene_stack.push_back(scene);
	this->set_scene(scene);
}

void window::pop_scene()
{
	ftrace();
	this->m_scene_stack.pop_back();
	this->set_scene(this->m_scene_stack.back());
}

void window::set_keyboard(onscreen_keyboard *okbd)
{
	ftrace();
	this->m_onscreen_kbd = okbd;
}

void window::show_keyboard(const std::string &value_name)
{
	ftrace();
	this->m_onscreen_kbd->show(value_name);
}

void window::set_channel_screen(channel_screen *ch_screen)
{
	ftrace();
	this->m_channel_screen = ch_screen;
}

void window::show_channel_screen(const std::string &channel_prefix)
{
	ftrace();
	this->m_channel_screen->show(channel_prefix);
}
