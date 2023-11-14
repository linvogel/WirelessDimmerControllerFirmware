#pragma once


#include <gui/window.hpp>
#include <gui/renderer.hpp>

#include <model/model.hpp>


namespace dim {
	namespace gui {
		class main_screen {
			window &m_window;
			renderer &m_renderer;
			dim::model::model &m_model;
			
			size_t m_scene;
			
			
		public:
			main_screen(window &window, renderer &renderer, dim::model::model &model);
			
			
		};
		
	}
}