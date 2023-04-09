#pragma once

#include <yaml-cpp/yaml.h>
#include <string>

#include "component.hpp"

namespace dim {
	namespace gui_builder {
		
		dim::gui::component* build_gui_from_file(dim::gui::renderer &renderer, const std::string &filename);
		dim::gui::component* build_gui_from_yaml(dim::gui::renderer &renderer, YAML::Node gui_node);
	}
}