#pragma once

#include "component.hpp"

namespace dim {
	namespace gui
	{
		
		class slider : public virtual component {
			quad2 m_rail;
			quad2 m_knob;
			
			float m_min_value;
			float m_max_value;
			float m_knob_width;
			float m_knob_height;
			float m_rail_width;
			
		public:
			slider(renderer &renderer, float x, float y, float w, float h, float min_val, float max_val, float rail_width, float knob_width, float knob_height);
			slider(renderer &renderer, float x, float y, float w, float h, float min_val, float max_val)
				: slider(renderer, x, y, w, h, min_val, max_val, 10, w-10, 60) {}
			
			virtual void draw_component(renderer &renderer) override;
			virtual void onMouseMove(float x, float y) override;
		};
		
	}
	
}