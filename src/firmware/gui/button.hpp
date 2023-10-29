#pragma once

#include "component.hpp"
#include <functional>
#include <string>

#include <model/model.hpp>

namespace dim {
	namespace gui {
		
		class button : public virtual component {
			std::function<void()> m_func;
			model::model &m_model;
			std::string m_value_name;
			
			renderer &m_renderer;
			
			vector4f m_act_color;
		public:
			button(model::model &model, std::string value_name, std::function<void()> func, renderer &renderer, float x, float y, float w, float h);
			
			virtual void set_callback(std::function<void()> func);
			
			virtual void draw_component(renderer &renderer) override;
			
			virtual void onLeftMouseDown(float x, float y) override;
			virtual void onLeftMouseUp(float x, float y) override;
		};
		
	}
}