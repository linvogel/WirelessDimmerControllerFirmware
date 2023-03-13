#include "knob.hpp"

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif
#define MODULE_NAME "knob"
#include "../logging.hpp"

#include <cmath>

using namespace dim::gui;
using namespace dim::math;


knob::knob(renderer &renderer, float x, float y, float size)
	: component(x, y, 1.0f, 1.0f, 0.0f, size, size)
	, m_knob(renderer, 5, 5, size-5, 5, size-5, size-5, 5, size-5)
{
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
	
	this->m_angle = 0;
	this->m_min_angle = static_cast<float>(-0.7 * PI);
	this->m_max_angle = static_cast<float>(0.7 * PI);
	this->m_sensitivity = 0.01f;
	this->m_grabbed = false;
	
	this->m_alpha_min = { std::sin(this->m_min_angle), std::cos(this->m_min_angle) };
	this->m_alpha_max = { std::sin(this->m_max_angle), std::cos(this->m_max_angle) };
}

void knob::draw_component(renderer &renderer)
{
	unsigned int program = this->m_shape->get_special_program();
	renderer.set_program(program);
	debug("hello 1");
	
	renderer.set_uniform_scalar(program, "u_angle", this->m_angle);
	vector4f bounds = {
		this->m_shape->get_bounds()(0) + this->m_shape->get_offset()(0),
		this->m_shape->get_bounds()(1) + this->m_shape->get_offset()(1),
		this->m_shape->get_bounds()(2),
		this->m_shape->get_bounds()(3)};
	renderer.set_uniform_vec4(program, "u_bounds", bounds.get_data());
	float *tmp = this->m_shape->get_offset().get_data();
	verbose("Bounds: %f %f %f %f", tmp[0], tmp[1], tmp[2], tmp[3]);
	debug("hello 11");
	renderer.set_uniform_vec2(program, "u_alpha_min", this->m_alpha_min.get_data());
	debug("hello 12");
	renderer.set_uniform_vec2(program, "u_alpha_max", this->m_alpha_max.get_data());
	debug("hello 13");
	renderer.set_uniform_vec2(program, "u_center_vec", (this->m_size * 0.5f).get_data());
	debug("hello 2");
	if (this->m_shape.get()) renderer.draw_shape(this->m_shape.get());
	
	renderer.push_proj();
	renderer.translate(this->m_size * 0.5f);
	renderer.rotate(this->m_angle);
	renderer.translate(this->m_size * -0.5f, true);
	this->m_knob.set_offset(this->m_shape->get_offset()(0), this->m_shape->get_offset()(1));
	//renderer.draw_shape(&(this->m_knob));
	renderer.pop_proj();
}

void knob::onMouseMove(float x, float y)
{
	if (this->m_grabbed) {
		this->m_angle = std::min(std::max(this->m_start_angle + (y - this->m_mousey)*this->m_sensitivity, this->m_min_angle), this->m_max_angle);
	}
}

void knob::onLeftMouseDown(float x, float y)
{
	this->m_mousey = y;
	this->m_start_angle = this->m_angle;
	this->m_grabbed = true;
}

void knob::onLeftMouseUp(float x, float y)
{
	this->m_grabbed = false;
}