#include "slider.hpp"

#define MODULE_NAME "slider"
#include "../logging.hpp"

using namespace dim::gui;
using namespace dim::math;


slider::slider(model::model &model, const std::string &value_name, renderer &renderer, float x, float y, float w, float h, float min_val, float max_val, float rail_width, float knob_width, float knob_height)
	: component(x, y, 1, 1, 0, w, h)
	, m_model(model)
	, m_value_name(value_name)
	, m_min_value(min_val)
	, m_max_value(max_val)
	, m_knob_width(knob_width)
	, m_knob_height(knob_height)
	, m_rail_width(rail_width)
	, m_rail(renderer, 0, 0, rail_width, 0, rail_width, h - knob_height, 0, h - knob_height)
	, m_knob(renderer, 0, 0, knob_width, 0, knob_width, knob_height, 0, knob_height)
	, m_drag_col({0.8f, 0.8f, 0.8f, 1.0f})
{
	ftrace();
	this->m_shape = std::make_shared<quad2>(renderer, 0.0f, 0.0f, w, 0.0f, w, h, 0.0f, h);
	this->m_shape->set_background_color(this->m_bg_color);
	this->m_shape->set_stroke_color(this->m_fg_color);
	this->m_shape->set_corner_radius(5);
	this->m_shape->set_stroke_weight(2);
	
	this->m_rail.set_background_color({0.8f, 0.8f, 0.8f, 1.0f});
	this->m_rail.set_stroke_color(this->m_fg_color);
	this->m_rail.set_corner_radius(5);
	this->m_rail.set_stroke_weight(2);
	
	this->m_knob.set_texture(new texture(renderer, "fader_knob"));
	this->m_knob.update_uv();
	
	this->m_min_value = min_val;
	this->m_max_value = max_val;
	this->m_len = h  - knob_height - 10;
	this->m_miny = 0;
	this->m_maxy = this->m_len;
	this->m_value = this->m_max_value;
	this->m_grabbed = false;
}

void slider::set_value_name(const std::string &value_name)
{
	ftrace();
	this->m_value_name = value_name;
}

void slider::draw_component(renderer &renderer)
{
	ftrace();
	renderer.draw_shape(this->m_shape.get());
	float position = static_cast<float>(this->m_model[this->m_value_name]);
	
	float rail_x = (this->m_size(0) - this->m_rail_width) * 0.5f;
	float rail_y = this->m_knob_height*0.5f;
	float knob_x = 5;
	float knob_y = 5;
	
	this->m_rail.set_offset(this->m_shape->get_offset()(0) + rail_x, this->m_shape->get_offset()(1) - rail_y);
	this->m_knob.set_offset(this->m_shape->get_offset()(0) + knob_x, this->m_shape->get_offset()(1));
	
	renderer.push_proj();
	renderer.translate({rail_x, rail_y}, true);
	renderer.draw_shape(&(this->m_rail));
	renderer.pop_proj();
	renderer.push_proj();
	renderer.translate({knob_x, knob_y + position}, true);
	renderer.draw_shape(&(this->m_knob));
	renderer.pop_proj();
}

void slider::onMouseMove(float x, float y)
{
	ftrace();
	if (this->m_grabbed) {
		float position = std::min(std::max(this->m_start_pos + y - this->m_mousey, this->m_miny), this->m_maxy);
		this->m_model[this->m_value_name] = position;
	}
}

void slider::onLeftMouseDown(float x, float y)
{
	ftrace();
	float position = static_cast<float>(this->m_model[this->m_value_name]);
	if (x >= 5 && this->m_size(0) - 5 >= x && y >= position + 5 && y <= position + this->m_knob_height + 5) {
		this->m_mousey = y;
		this->m_start_pos = position;
		this->m_grabbed = true;
		this->m_knob.set_background_color(this->m_drag_col);
	}
}

void slider::onLeftMouseUp(float x, float y)
{
	ftrace();
	this->m_grabbed = false;
	this->m_knob.set_background_color(this->m_bg_color);
}
