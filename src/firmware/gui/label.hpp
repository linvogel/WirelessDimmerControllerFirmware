#pragma once

#include "component.hpp"

namespace dim {
	namespace gui {
		
		class label : public virtual component {
			std::string m_string;
			float m_font_size;
			
		public:
			label(renderer &renderer, float x, float y, float w, float h, std::string string, float font_size);
			
			static component* from_yaml(renderer &renderer, YAML::Node root);
			
			virtual void draw_component(renderer &renderer) override;
			
			virtual void set_text(std::string text);
		};
		
	}
}