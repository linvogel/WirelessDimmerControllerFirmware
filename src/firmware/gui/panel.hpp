#pragma once

#include "component.hpp"
#include "../math/matrix.hpp"

namespace dim {
	namespace gui {
		
		class panel : public virtual component {
			
		public:
			panel(renderer &renderer, vector2f pos, vector2f scale, float angle, vector2f size);
			panel(renderer &renderer, float x, float y, float w, float h) : panel(renderer, {x, y}, {1.0f, 1.0f}, 0, {w, h}) {}
			
		};
		
	}
}