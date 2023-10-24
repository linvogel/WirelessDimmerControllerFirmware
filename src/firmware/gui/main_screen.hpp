#pragma once


#include <gui/panel.hpp>
#include <gui/renderer.hpp>


namespace dim {
	namespace gui {
		
		class main_screen : public virtual panel {
			
		public:
			main_screen(renderer &renderer);
			~main_screen();
		};
		
	}
}