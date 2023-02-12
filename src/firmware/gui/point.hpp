#pragma once

#include "../math/matrix.hpp"


namespace dimmer {
	namespace gui {
		class point {
		private:
		public:
			point(math::vector2f pos, math::vector3f color);
			point(math::vector2f pos) : point(pos, math::vector3f({0, 0, 0})) {}
			point() : point(math::vector2f({0, 0})) {}
		};
	}
}