#include "panel.hpp"

#include "guibuilder.hpp"

using namespace dim::gui;
using namespace dim::math;
using namespace dim::gui_builder;

panel::panel(renderer &renderer, vector2f pos, vector2f scale, float angle, vector2f size) : component(pos, scale, angle, size)
{
	this->m_shape = std::make_shared<quad2>(renderer, 0.0f, 0.0f, size(0), 0.0f, size(0), size(1), 0.0f, size(1));
	this->m_shape->set_corner_radius(5.0f);
	this->m_shape->set_stroke_weight(2.0f);
	this->m_shape->set_background_color(this->m_bg_color);
	this->m_shape->set_stroke_color(this->m_fg_color);
}

component* panel::from_yaml(renderer &renderer, YAML::Node root)
{
	debug("building panel");
	// read position
	float x = root["bounds"]["x"].as<float>();
	float y = root["bounds"]["y"].as<float>();
	float w = root["bounds"]["w"].as<float>();
	float h = root["bounds"]["h"].as<float>();
	
	panel* pnl = new panel(renderer, x, y, w, h);
	
	if (root["children"].IsMap()) {
		for (auto chld : root["children"]) {
			// Note: this is a short dictionary
			std::string key = chld.first.as<std::string>();
			YAML::Node value = chld.second;
			pnl->add(build_gui_from_yaml(renderer, value));
		}
	} else {
		warn("Malformed children list!");
	}
	
	return pnl;
}