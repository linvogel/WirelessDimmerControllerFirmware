#pragma once

#include <string>

#include "../math/matrix.hpp"

namespace dim {
	namespace gui {
		class component;
	}
	namespace event {
		
		class event {
		protected:
			dim::math::vector2f m_location;
			
		public:
			event() { this->m_location = {0, 0}; }
			event(dim::math::vector2f location);
			virtual ~event();
			
			virtual dim::math::vector2f& get_location() final;
			
			virtual void handle_for_component(dim::gui::component* comp);
		};
		
	}
}
