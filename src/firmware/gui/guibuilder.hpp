#pragma once

#include <yaml-cpp/yaml.h>
#include <string>

#include "component.hpp"

namespace dim {
	namespace gui_builder {
		
		dim::gui::component* build_gui_from_yaml(YAML::Node gui_node);
		dim::gui::component* build_gui_from_file(const std::string &filename);
		
	}
}