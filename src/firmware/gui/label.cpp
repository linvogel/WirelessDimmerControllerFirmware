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

void label::set_text(std::string text)
{
	this->m_string = text;
}

component* label::from_yaml(renderer &renderer, YAML::Node root)
{
	float x = root["bounds"]["x"].as<float>();
	float y = root["bounds"]["y"].as<float>();
	float w = root["bounds"]["w"].as<float>();
	float h = root["bounds"]["h"].as<float>();
	
	std::string text = root["text"].as<std::string>();
	float font_size = root["font_size"].as<float>();
	
	label *lbl = new label(renderer, x, y, w, h, text, font_size);
	
	return lbl;
}