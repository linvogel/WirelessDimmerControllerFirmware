#include "panel.hpp"

using namespace dim::gui;
using namespace dim::math;

panel::panel(renderer &renderer, vector2f pos, vector2f scale, float angle, vector2f size) : component(pos, scale, angle, size)
{
	ftrace();
	this->m_shape = std::make_shared<quad2>(renderer, 0.0f, 0.0f, size(0), 0.0f, size(0), size(1), 0.0f, size(1));
	this->m_shape->set_corner_radius(5.0f);
	this->m_shape->set_stroke_weight(2.0f);
	this->m_shape->set_background_color(this->m_bg_color);
	this->m_shape->set_stroke_color(this->m_fg_color);
}