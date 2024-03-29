#include "input_controller.hpp"

#include "../logging.hpp"

#include <map>
#include <memory>

using namespace dim::gui;

std::map<GLFWwindow*,dim::in::input_controller*> s_controllers;

void s_key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	ftrace();
	auto it = s_controllers.find(win);
	if (it != s_controllers.end()) s_controllers.at(win)->key_callback(key, scancode, action, mods);
	else warn("glfwKeyCallback cannot be handled.");
}

void s_cursor_pos_callback(GLFWwindow *win, double x, double y)
{
	ftrace();
	auto it = s_controllers.find(win);
	if (it != s_controllers.end()) s_controllers.at(win)->mouse_callback(x, y);
	else warn("glfwCursorPosCallback cannot be handled.");
}

void s_mouse_button_callback(GLFWwindow *win, int button, int action, int mods)
{
	ftrace();
	auto it = s_controllers.find(win);
	if (it != s_controllers.end()) s_controllers.at(win)->mouse_button_callback(button, action, mods);
	else warn("glfwMouseButtonCallback cannot be handled.");
}

dim::in::input_controller::input_controller()
{
	ftrace();
	this->m_window = nullptr;
	this->m_comp = nullptr;

	for (int i = 0; i < N_KEYS; i++) this->m_keys[i] = 0;
	for (int i = 0; i < N_BUTTONS; i++) this->m_buttons[i] = 0;
	this->m_mousex = 0;
	this->m_mousey = 0;
}

dim::in::input_controller::input_controller(GLFWwindow *window, dim::gui::component *comp)
{
	ftrace();
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
	ftrace();
	if (this->m_window != nullptr) {
		s_controllers.erase(this->m_window);
		debug("Erasing input_controller: %p", this->m_window);
	}
}

void dim::in::input_controller::key_callback(int key, int scancode, int action, int mods)
{
	ftrace();
	if (action == GLFW_PRESS) {
		if (this->m_comp->get_focussed()) this->m_comp->get_focussed()->onKeyPressed(key, mods);
	} else if (action == GLFW_REPEAT) {
		if (this->m_comp->get_focussed()) this->m_comp->get_focussed()->onKeyTyped(key, mods);
	} else if (action == GLFW_RELEASE) {
		if (this->m_comp->get_focussed()) this->m_comp->get_focussed()->onKeyTyped(key, mods);
		if (this->m_comp->get_focussed()) this->m_comp->get_focussed()->onKeyReleased(key, mods);
	}
}

void dim::in::input_controller::mouse_callback(double x, double y)
{
	ftrace();
	component* old = this->m_comp->hit_children(this->m_mousex, this->m_mousey);
	float oldx = this->m_mousex;
	float oldy = this->m_mousey;
	this->m_mousex = (float)x;
	this->m_mousey = (float)y;
	component* cur = this->m_comp->hit_children(this->m_mousex, this->m_mousey);
	
	vector2f offset = { 0, 0 };
	if (this->m_comp->get_focussed()) {
		for (component *c = this->m_comp->get_focussed(); c; c = c->get_parent()) offset += c->get_position();
		this->m_comp->get_focussed()->onMouseMove(this->m_mousex - offset(0), this->m_mousey - offset(1));
	}
	offset = { 0, 0 };
	if (cur) {
		for (component *c = cur; c; c = c->get_parent()) offset += c->get_position();
		cur->onMouseMove(this->m_mousex - offset(0), this->m_mousey - offset(1));
	}
	
	// if old and cur are the same, nothing changed, so return
	if (cur == old) return;
	
	// if old is not hit anymore, exit old and all affected parents
	if (old && !old->hit(this->m_mousex, this->m_mousey)) {
		for (component* c = old; c != nullptr && !c->hit(this->m_mousex, this->m_mousey); c = c->get_parent()) c->onMouseExit();
	}
	
	// if cur wasn't hit before, enter it and all affected parents
	if (cur && !cur->hit(oldx, oldy)) {
		for (component* c = cur; c != nullptr && !c->hit(oldx, oldy); c = c->get_parent()) c->onMouseEnter();
	}
}

void dim::in::input_controller::mouse_button_callback(int button, int action, int mods)
{
	ftrace();
	if (action == GLFW_PRESS) {
		// mark button as pressed
		this->m_buttons[button] = true;
		// update focussed component
		component *hit_component = this->m_comp->hit_children(this->m_mousex, this->m_mousey);
		this->m_comp->focus(hit_component);
		vector2f offset = {0, 0};
		for (component* c = hit_component; c; c = c->get_parent()) offset += c->get_position();
		
		// call event handlers
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			hit_component->onLeftMouseDown(this->m_mousex - offset(0), this->m_mousey - offset(1));
			this->m_last_hit_left_down = hit_component;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			hit_component->onRightMouseDown(this->m_mousex - offset(0), this->m_mousey - offset(1));
			this->m_last_hit_right_down = hit_component;
		}
	} else if (action == GLFW_RELEASE) {
		// mark button as unpressed
		this->m_buttons[button] = false;
		// call event handlers
		vector2f offset = {0, 0};
		component *hit_component = this->m_comp->hit_children(this->m_mousex, this->m_mousey);
		for (component* c = hit_component; c; c = c->get_parent()) offset += c->get_position();
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (this->m_last_hit_left_down != nullptr)
				this->m_last_hit_left_down->onLeftMouseUp(this->m_mousex - offset(0), this->m_mousey - offset(1));
			this->m_last_hit_left_down = nullptr;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (this->m_last_hit_right_down != nullptr)
				this->m_last_hit_right_down->onRightMouseUp(this->m_mousex - offset(0), this->m_mousey - offset(1));
			this->m_last_hit_right_down = nullptr;
		}
	}
}