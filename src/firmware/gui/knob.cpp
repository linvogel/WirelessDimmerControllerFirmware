#include "knob.hpp"

#define MODULE_NAME "knob"
#include "../logging.hpp"

using namespace dim::gui;
using namespace dim::math;


knob::knob(renderer &renderer, float x, float y, float size)
	: component(x, y, 1.0f, 1.0f, 0.0f, size, size)
	, m_knob(renderer, 0, 0, size, 0, size, size, 0, size)
{
	// create base shape with outline and fully rounded corners
	this->m_shape = std::make_shared<quad2>(renderer, 0.0f, 0.0f, size, 0.0f, size, size, 0.0f, size);
	this->m_shape->set_corner_radius(size * 0.5f);
	this->m_shape->set_background_color(this->m_bg_color);
	this->m_shape->set_stroke_color(this->m_fg_color);
	
	
}
