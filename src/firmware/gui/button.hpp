#pragma once

#include "component.hpp"
#include <functional>
#include <string>

#include <model/model.hpp>

namespace dim {
	namespace gui {
		
		class button : public virtual component {
			std::function<void()> m_func;
			model::model_value *m_mval;
			
			renderer &m_renderer;
			
			vector4f m_act_color;
		public:
			button(model::model_value *mval, std::function<void()> func, renderer &renderer, float x, float y, float w, float h);
			
			static component* from_yaml(renderer &renderer, YAML::Node root, model::model &model);
			
			virtual void set_callback(std::function<void()> func);
			
			virtual void draw_component(renderer &renderer) override;
			
			virtual void onLeftMouseDown(float x, float y) override;
			virtual void onLeftMouseUp(float x, float y) override;
		};
		
	}
}