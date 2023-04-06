#include "guibuilder.hpp"

#include <vector>

#define MODULE_NAME "GUI_BUILDER"
#include "../logging.hpp"


dim::gui::component* dim::gui_builder::build_gui_from_file(const std::string &filename)
{
	try {
		std::vector<YAML::Node> nodes = YAML::LoadAllFromFile(filename);
		
		if  (nodes.size() == 0) {
			error("Gui file does not contain gui data: %s", filename);
			return nullptr;
		}
		
		if (nodes.size() > 1) {
			warn("Multiple yaml nodes in gui file, uncertain if choosing the correct one!");
		}
		
		return build_gui_from_yaml(nodes[0]);
		
	} catch (std::exception e) {
		error("Unable to build gui from file: %s", filename);
		return nullptr;
	}
}

dim::gui::component* dim::gui_builder::build_gui_from_yaml(YAML::Node gui_node)
{
	if (!gui_node.IsMap()) {
		error("Gui Node must be of map type!");
		return nullptr;
	}
	
	for (YAML::iterator it = gui_node.begin(); it != gui_node.end(); ++it) {
		info("Tag: %s", it->Tag().c_str());
	}
}