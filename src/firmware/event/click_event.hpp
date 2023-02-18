#pragma once

#include "event.hpp"
#include "../math/matrix.hpp"

namespace dim {
	namespace gui {
		class component;
	}
	namespace event {
		
		class event;
		
		class click_event : public virtual event {
		public:
			click_event(dim::math::vector2f location) : event(location) {}
			
			virtual void handle_for_component(dim::gui::component* comp) override;
		};
		
	}
}