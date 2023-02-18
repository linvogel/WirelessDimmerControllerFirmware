#include "input_controller.hpp"

#include "../logging.hpp"

#include <map>
#include <memory>

std::map<GLFWwindow*,dim::in::input_controller*> s_controllers;

void s_key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	auto it = s_controllers.find(win);
	if (it != s_controllers.end()) s_controllers.at(win)->key_callback(key, scancode, action, mods);
	else warn("glfwKeyCallback cannot be handled.");
}

void s_cursor_pos_callback(GLFWwindow *win, double x, double y)
{
	auto it = s_controllers.find(win);
	if (it != s_controllers.end()) s_controllers.at(win)->mouse_callback(x, y);
	else warn("glfwCursorPosCallback cannot be handled.");
}

void s_mouse_button_callback(GLFWwindow *win, int button, int action, int mods)
{
	auto it = s_controllers.find(win);
	if (it != s_controllers.end()) s_controllers.at(win)->mouse_button_callback(button, action, mods);
	else warn("glfwMouseButtonCallback cannot be handled.");
}

dim::in::input_controller::input_controller()
{
	this->m_window = nullptr;
	this->m_comp = nullptr;

	for (int i = 0; i < N_KEYS; i++) this->m_keys[i] = 0;
	for (int i = 0; i < N_BUTTONS; i++) this->m_buttons[i] = 0;
	this->m_mousex = 0;
	this->m_mousey = 0;
}

dim::in::input_controller::input_controller(GLFWwindow *window, dim::gui::component *comp)
{
	this->m_window = window;
	this->m_comp = comp;
	debug("Creating input_controller: %p", window);
	
	for (int i = 0; i < N_KEYS; i++) this->m_keys[i] = 0;
	for (int i = 0; i < N_BUTTONS; i++) this->m_buttons[i] = 0;
	this->m_mousex = 0;
	this->m_mousey = 0;
	
	s_controllers.emplace(this->m_window, this);
	
	glfwSetCursorPosCallback(this->m_window, s_cursor_pos_callback);
	glfwSetKeyCallback(this->m_window, s_key_callback);
	glfwSetMouseButtonCallback(this->m_window, s_mouse_button_callback);
}

dim::in::input_controller::~input_controller()
{
	if (this->m_window != nullptr) {
		s_controllers.erase(this->m_window);
		debug("Erasing input_controller: %p", this->m_window);
	}
}

void dim::in::input_controller::key_callback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		debug("GLFW_PRESS: %c", key);
	} else if (action == GLFW_REPEAT) {
		debug("GLFW_REPEAT: %c", key);
	} else if (action == GLFW_RELEASE) {
		debug("GLFW_RELEASE: %c", key);
	}
}

void dim::in::input_controller::mouse_callback(double x, double y)
{
	this->m_mousex = (float)x;
	this->m_mousey = (float)y;
}

void dim::in::input_controller::mouse_button_callback(int button, int action, int mods)
{
	if (this->m_comp) this->m_comp->handle_generic_event(std::make_shared<dim::event::click_event>(dim::math::vector2f({this->m_mousex, this->m_mousey})));
}