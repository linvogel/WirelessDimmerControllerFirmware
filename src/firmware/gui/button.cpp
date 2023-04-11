#include "button.hpp"

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif
#define MODULE_NAME "button"
#include "../logging.hpp"

#include "guibuilder.hpp"

using namespace dim::gui;
using namespace dim::math;
using namespace dim::gui_builder;

button::button(std::string text, std::function<void()> func, renderer &renderer, float x, float y, float w, float h)
	: component(x, y, 1.0f, 1.0f, 0.0f, w, h), m_func(func), m_text(text), m_renderer(renderer)
{
	this->m_act_color = vector4f({0.0f, 0.218f, 0.411f, 1.0f});
	this->m_shape = std::make_shared<quad2>(this->m_renderer, 0.0f, 0.0f, w, 0.0f, w, h, 0.0f, h);
	this->m_shape->set_corner_radius(5.0f);
	this->m_shape->set_stroke_weight(2.0f);
	this->m_shape->set_background_color(this->m_bg_color);
	this->m_shape->set_stroke_color(this->m_fg_color);
}

void button::draw_component(renderer &renderer)
{
	this->component::draw_component(renderer);
	
	vector2f ts = renderer.get_text_size(this->m_text, 1.0f);
	float ws = (this->m_size(0) * 0.8f) / ts(0);
	float hs = (this->m_size(1) * 0.8f);
	float size = std::min(ws, hs);
	renderer.draw_text_centered(this->m_text, this->m_size(0) * 0.5f, this->m_size(1) * 0.5f, size);
}

void button::onLeftMouseDown(float x, float y)
{
	this->m_shape->set_background_color(this->m_act_color);
}

void button::onLeftMouseUp(float x, float y)
{
	this->m_shape->set_background_color(this->m_bg_color);
	if (0 <= x && 0 <= y && this->m_size(0) >= x && this->m_size(1) >= y)
		if (this->m_func) this->m_func();
}

void button::set_callback(std::function<void()> func)
{
	this->m_func = func;
}

component* button::from_yaml(renderer &renderer, YAML::Node root)
{
	debug("building button...");
	// read position
	float x = root["bounds"]["x"].as<float>();
	float y = root["bounds"]["y"].as<float>();
	float w = root["bounds"]["w"].as<float>();
	float h = root["bounds"]["h"].as<float>();
	
	std::string text = root["text"].as<std::string>();
	
	button* btn = new button(text, []() { info("Uninitialized button function!"); }, renderer, x, y, w, h);
	btn->set_callback([btn]() { info("Button pressed: '%s'", btn->m_text.c_str()); });
	return btn;
}