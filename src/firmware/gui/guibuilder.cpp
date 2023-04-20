#include "guibuilder.hpp"

#include <vector>

#define MODULE_NAME "GUI_BUILDER"
#include "../logging.hpp"

#include "panel.hpp"
#include "button.hpp"
#include "knob.hpp"
#include "slider.hpp"
#include "label.hpp"

using namespace dim::gui;


dim::gui::component* dim::gui_builder::build_gui_from_file(renderer &renderer, const std::string &filename)
{
	try {
		std::vector<YAML::Node> nodes = YAML::LoadAllFromFile(filename);
		
		if  (nodes.size() == 0) {
			error("Gui file does not contain gui data: %s", filename);
			return nullptr;
		}
		
		int i = -1;
		for (int k = 0; k < nodes.size(); k++) {
			if (nodes[k]["gui"]) {
				if (i != -1) warn("Multiple yaml nodes in gui file, uncertain if choosing the correct one!");
				else i = k;
			}
		}
		
		if (i == -1) {
			error("Gui file does not contain valid gui node: %s", filename.c_str());
			return nullptr;
		}
		
		return build_gui_from_yaml(renderer, nodes[i]["gui"]);
		
	} catch (std::exception e) {
		error("Unable to build gui from file: %s", filename.c_str());
		error("Error: %s", e.what());
		return nullptr;
	}
}

dim::gui::component* dim::gui_builder::build_gui_from_yaml(renderer &renderer, YAML::Node root)
{
	if (!root.IsMap()) {
		error("Gui Node must be of map type!");
		return nullptr;
	}
	
	std::string type = root["type"].as<std::string>();
	if (type == "panel") {
		return panel::from_yaml(renderer, root);
	} else if (type == "button") {
		return button::from_yaml(renderer, root);
	} else if (type == "knob") {
		return knob::from_yaml(renderer, root);
	} else if (type == "label") {
		return label::from_yaml(renderer, root);
	} else if (type == "slider") {
		return slider::from_yaml(renderer, root);
	}
	
	
	warn("Unhandled type: %s", type.c_str());
	return nullptr;
}