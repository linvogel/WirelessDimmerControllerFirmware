#include "label.hpp"

using namespace dim::gui;
using namespace dim::math;

label::label(renderer &renderer, float x, float y, float w, float h, std::string str, float font_size)
	: component(x, y, 1, 1, 0, w, h)
	, m_string(str)
	, m_font_size(font_size)
{}

void label::draw_component(renderer &renderer)
{
	renderer.draw_text_centered(this->m_string, this->m_size(0)*0.5f, this->m_size(1)*0.5f, this->m_font_size);
}