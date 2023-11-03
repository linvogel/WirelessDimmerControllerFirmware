#include "knob.hpp"

#define MODULE_NAME "knob"
#include "../logging.hpp"

#include <cmath>

using namespace dim::gui;
using namespace dim::math;


knob::knob(model::model &model, std::string value_name, renderer &renderer, float x, float y, float size)
	: component(x, y, 1.0f, 1.0f, 0.0f, size, size)
	, m_knob(renderer, 5, 5, size-5, 5, size-5, size-5, 5, size-5)
	, m_model(model)
	, m_value_name(value_name)
{
	ftrace();
	// create base shape with outline and fully rounded corners
	this->m_shape = std::make_shared<quad2>(renderer, 0.0f, 0.0f, size, 0.0f, size, size, 0.0f, size);
	this->m_shape->set_corner_radius(size * 0.5f);
	this->m_shape->set_background_color(this->m_bg_color);
	this->m_shape->set_stroke_color(this->m_fg_color);
	this->m_shape->set_stroke_weight(2);
	this->m_shape->set_special_program(renderer.get_knob_program());
	
	// create knob shape
	m_knob.set_texture(new texture(renderer, "rotary_knob"));
	m_knob.update_uv();
	
	this->m_min_angle = static_cast<float>(-0.7 * PI);
	this->m_max_angle = static_cast<float>(0.7 * PI);
	this->m_sensitivity = 0.01f;
	this->m_grabbed = false;
	
	this->m_alpha_min = { std::sin(this->m_min_angle), std::cos(this->m_min_angle) };
	this->m_alpha_max = { std::sin(this->m_max_angle), std::cos(this->m_max_angle) };
}

void knob::set_value_name(const std::string &value_name)
{
	ftrace();
	this->m_value_name = value_name;
}

void knob::draw_component(renderer &renderer)
{
	ftrace();
	unsigned int program = this->m_shape->get_special_program();
	renderer.set_program(program);
	
	float angle = static_cast<float>(this->m_model[this->m_value_name]); // TODO: this should probably be translated a bit
	
	renderer.set_uniform_scalar(program, "u_angle", -angle);
	vector4f bounds = {
		this->m_shape->get_bounds()(0) + this->m_shape->get_offset()(0),
		this->m_shape->get_bounds()(1) + this->m_shape->get_offset()(1),
		this->m_shape->get_bounds()(2),
		this->m_shape->get_bounds()(3)};
	renderer.set_uniform_vec4(program, "u_bounds", bounds.get_data());
	renderer.set_uniform_vec2(program, "u_alpha_min", this->m_alpha_min.get_data());
	renderer.set_uniform_vec2(program, "u_alpha_max", this->m_alpha_max.get_data());
	renderer.set_uniform_vec2(program, "u_center_vec", (this->m_size * 0.5f).get_data());
	if (this->m_shape.get()) renderer.draw_shape(this->m_shape.get());
	
	renderer.push_proj();
	renderer.translate(this->m_size * 0.5f);
	renderer.rotate(angle);
	renderer.translate(this->m_size * -0.5f, true);
	this->m_knob.set_offset(this->m_shape->get_offset()(0), this->m_shape->get_offset()(1));
	renderer.draw_shape(&(this->m_knob));
	renderer.pop_proj();
}

void knob::onMouseMove(float x, float y)
{
	ftrace();
	if (this->m_grabbed) {
		this->m_model[this->m_value_name] = std::min(std::max(this->m_start_angle + (y - this->m_mousey)*this->m_sensitivity, this->m_min_angle), this->m_max_angle);
	}
}

void knob::onLeftMouseDown(float x, float y)
{
	ftrace();
	this->m_mousey = y;
	this->m_start_angle = static_cast<float>(this->m_model[this->m_value_name]);
	this->m_grabbed = true;
}

void knob::onLeftMouseUp(float x, float y)
{
	ftrace();
	this->m_grabbed = false;
}