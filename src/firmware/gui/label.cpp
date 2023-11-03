#include "label.hpp"

using namespace dim::gui;
using namespace dim::math;

label::label(model::model &model, std::string value_name, renderer &renderer, float x, float y, float w, float h, float font_size)
	: component(x, y, 1, 1, 0, w, h)
	, m_model(model)
	, m_value_name(value_name)
	, m_font_size(font_size)
{
	ftrace();
}

void label::set_value_name(const std::string &value_name)
{
	ftrace();
	this->m_value_name = value_name;
}

void label::draw_component(renderer &renderer)
{
	ftrace();
	renderer.draw_text_centered(static_cast<std::string>(this->m_model[this->m_value_name]), this->m_size(0)*0.5f, this->m_size(1)*0.5f, this->m_font_size);
}
