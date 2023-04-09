#pragma once

#include "component.hpp"
#include <functional>
#include <string>

namespace dim {
	namespace gui {
		
		class button : public virtual component {
			std::function<void()> m_func;
			std::string m_text;
			
			renderer &m_renderer;
			
			vector4f m_act_color;
		public:
			button(std::string text, std::function<void(void)> func, renderer &renderer, float x, float y, float w, float h);
			
			static component* from_yaml(renderer &renderer, YAML::Node root);
			
			virtual void draw_component(renderer &renderer) override;
			
			virtual void onLeftMouseDown(float x, float y) override;
			virtual void onLeftMouseUp(float x, float y) override;
		};
		
	}
}