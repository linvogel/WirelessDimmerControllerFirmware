#pragma once

#include "component.hpp"
#include "model/model.hpp"


namespace dim {
	namespace gui {
		
		template<typename T, T MIN, T MAX>
		class knob : public virtual component {
			vector2f m_alpha_min;
			vector2f m_alpha_max;
			float m_min_angle;
			float m_max_angle;
			float m_start_angle;
			float m_mousey;
			float m_sensitivity;
			bool m_grabbed;
			
			textured_quad2 m_knob;
			
			model::model &m_model;
			std::string m_value_name;
		public:
			knob(model::model &model, std::string value_name, renderer &renderer, float x, float y, float size);
			
			void set_value_name(const std::string &value_name);
			
			virtual void draw_component(renderer &renderer) override;
			
			virtual void onLeftMouseDown(float x, float y) override;
			virtual void onLeftMouseUp(float x, float y) override;
			virtual void onMouseMove(float x, float y) override;
		};
		
	}
}