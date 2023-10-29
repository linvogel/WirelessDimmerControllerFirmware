#pragma once

#include "component.hpp"
#include "model/model.hpp"

namespace dim {
	namespace gui {
		
		class label : public virtual component {
			model::model &m_model;
			std::string m_value_name;
			float m_font_size;
			
		public:
			label(model::model &model, std::string m_value_name, renderer &renderer, float x, float y, float w, float h, float font_size);
			
			virtual void draw_component(renderer &renderer) override;
		};
		
	}
}