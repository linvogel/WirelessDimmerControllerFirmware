#pragma once

#include "window.hpp"
#include "button.hpp"
#include "label.hpp"
#include "knob.hpp"

#include "model/model.hpp"


namespace dim {
	namespace gui {
		class window;
		
		class channel_screen {
			window &m_window;
			model::model &m_model;
			
			size_t m_scene;
			button m_name;
			label m_address;
			knob m_address_knob;
			
		public:
			channel_screen(renderer &renderer, window &window, model::model &model);
			
			void show(const std::string &channel_name);
		};
		
	}
}
