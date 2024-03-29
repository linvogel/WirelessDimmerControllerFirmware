#include "knob.hpp"

#include <cmath>
#include <limits>

#define MODULE_NAME "knob"
#include "../logging.hpp"


using namespace dim::gui;
using namespace dim::math;

template class knob<uint8_t, 0, 255>;
template class knob<float, 0.0f, 1.0f>;

template<typename T, T MIN, T MAX>
knob<T, MIN, MAX>::knob(model::model &model, std::string value_name, renderer &renderer, float x, float y, float size)
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
	
	this->m_start_angle = 0;
	this->m_min_angle = static_cast<float>(-0.7 * PI);
	this->m_max_angle = static_cast<float>(0.7 * PI);
	this->m_sensitivity = 0.01f;
	this->m_grabbed = false;
	
	this->m_alpha_min = { std::sin(this->m_min_angle), std::cos(this->m_min_angle) };
	this->m_alpha_max = { std::sin(this->m_max_angle), std::cos(this->m_max_angle) };
}

template<typename T, T MIN, T MAX>
void knob<T, MIN, MAX>::set_value_name(const std::string &value_name)
{
	ftrace();
	this->m_value_name = value_name;
}

template<typename T, T MIN, T MAX>
void knob<T, MIN, MAX>::draw_component(renderer &renderer)
{
	ftrace();
	unsigned int program = this->m_shape->get_special_program();
	renderer.set_program(program);
	
	T value = static_cast<T>(this->m_model[this->m_value_name]);
	float interpolation = 1.0f - static_cast<float>(value - MIN) / static_cast<float>(MAX - MIN);
	float angle = this->m_min_angle + interpolation * (this->m_max_angle - this->m_min_angle);
	
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

template<typename T, T MIN, T MAX>
void knob<T, MIN, MAX>::onMouseMove(float x, float y)
{
	ftrace();
	if (this->m_grabbed) {
		float angle = std::min(std::max(this->m_start_angle + (y - this->m_mousey)*this->m_sensitivity, this->m_min_angle), this->m_max_angle);
		float interpolation = 1.0f - (angle - this->m_min_angle) / (this->m_max_angle - this->m_min_angle);
		float value = static_cast<float>(MIN) + interpolation * static_cast<float>(MAX - MIN);
		this->m_model[this->m_value_name] = static_cast<T>(std::round(value));
	}
}

template<typename T, T MIN, T MAX>
void knob<T, MIN, MAX>::onLeftMouseDown(float x, float y)
{
	ftrace();
	this->m_mousey = y;
	T value = static_cast<T>(this->m_model[this->m_value_name]);
	float interpolation = 1.0f - static_cast<float>(value - MIN) / static_cast<float>(MAX - MIN);
	this->m_start_angle = this->m_min_angle + interpolation * (this->m_max_angle - this->m_min_angle);
	this->m_grabbed = true;
}

template<typename T, T MIN, T MAX>
void knob<T, MIN, MAX>::onLeftMouseUp(float x, float y)
{
	ftrace();
	this->m_grabbed = false;
}