#pragma once

#include "component.hpp"



namespace dim {
	namespace gui {
		
		class knob : public virtual component {
			float m_value;
			float m_min_angle;
			float m_max_angle;
			float m_angle;
			float m_start_angle;
			float m_mousey;
			float m_sensitivity;
			bool m_grabbed;
			
			textured_quad2 m_knob;
		public:
			knob(renderer &renderer, float x, float y, float size);
			
			virtual void draw_component(renderer &renderer) override;
			
			virtual void onLeftMouseDown(float x, float y) override;
			virtual void onLeftMouseUp(float x, float y) override;
			virtual void onMouseMove(float x, float y) override;
		};
		
	}
}