#include "slider.hpp"

#include "../logging.hpp"

using namespace dim::gui;
using namespace dim::math;


slider::slider(renderer &renderer, float x, float y, float w, float h, float min_val, float max_val, float rail_width, float knob_width, float knob_height)
	: component(x, y, 1, 1, 0, w, h)
	, m_min_value(min_val)
	, m_max_value(max_val)
	, m_knob_width(knob_width)
	, m_knob_height(knob_height)
	, m_rail_width(rail_width)
	, m_rail(renderer, 0, 0, rail_width, 0, rail_width, h - knob_height, 0, h - knob_height)
	, m_knob(renderer, 0, 0, knob_width, 0, knob_width, knob_height, 0, knob_height)
{
	this->m_shape = std::make_shared<quad2>(renderer, 0, 0, w, 0, w, h, 0, h);
	this->m_shape->set_background_color(this->m_bg_color);
	this->m_shape->set_stroke_color(this->m_fg_color);
	this->m_shape->set_corner_radius(5);
	this->m_shape->set_stroke_weight(2);
	
	this->m_rail.set_background_color({0.8f, 0.8f, 0.8f, 1.0f});
	this->m_rail.set_stroke_color(this->m_fg_color);
	this->m_rail.set_corner_radius(5);
	this->m_rail.set_stroke_weight(2);
	
	this->m_knob.set_background_color(this->m_bg_color);
	this->m_knob.set_stroke_color(this->m_fg_color);
	this->m_knob.set_corner_radius(5);
	this->m_knob.set_stroke_weight(2);
	
	this->m_min_value = min_val;
	this->m_max_value = max_val;
}

void slider::draw_component(renderer &renderer)
{
	trace("slider begin");
	renderer.draw_shape(this->m_shape.get());
	
	
	float rail_x = (this->m_size(0) - this->m_rail_width) * 0.5f;
	float rail_y = this->m_knob_height*0.5f;
	float knob_x = 5;
	float knob_y = 5;
	
	this->m_rail.set_offset(this->m_shape->get_offset()(0)+rail_x, this->m_shape->get_offset()(1) + rail_y);
	this->m_knob.set_offset(this->m_shape->get_offset()(0)+knob_x, this->m_shape->get_offset()(1) + knob_y);
	
	renderer.translate({rail_x, rail_y}, true);
	renderer.draw_shape(&(this->m_rail));
	renderer.translate({knob_x-rail_x, knob_y-rail_y}, true);
	renderer.draw_shape(&(this->m_knob));
	trace("slider end");
}

void slider::onMouseMove(float x, float y)
{
	
}