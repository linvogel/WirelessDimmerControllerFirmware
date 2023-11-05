#include "channel_screen.hpp"

using namespace dim::gui;


channel_screen::channel_screen(renderer &renderer, window &window, model::model &model)
	: m_window(window)
	, m_model(model)
	, m_name(model, "tmp.dummy_string", [](){}, renderer, 10, 10, 780, 30)
	, m_address(model, "tmp.dummy_string", renderer, 10, 50, 300, 30, 16)
	, m_address_knob(model, "tmp.dummy_double", renderer, 60, 90, 200)
{
	ftrace();
	// setup scene
	this->m_scene = this->m_window.create_scene();
	this->m_window.set_scene(this->m_scene);
	this->m_window.add(&(this->m_name));
	this->m_window.add(&(this->m_address));
	this->m_window.add(&(this->m_address_knob));
}


void channel_screen::show(const std::string &channel_prefix)
{
	ftrace();
	this->m_name.set_value_name(channel_prefix + ".name");
	this->m_name.set_callback([this, channel_prefix]() {
		this->m_window.show_keyboard(channel_prefix + ".name");
	});
	this->m_address.set_value_name(channel_prefix + ".address");
	this->m_address_knob.set_value_name(channel_prefix + ".address");
	
	this->m_window.push_scene(this->m_scene);
}