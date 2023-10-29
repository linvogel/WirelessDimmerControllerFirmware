#pragma once

#include <gui/component.hpp>
#include <gui/label.hpp>
#include <gui/button.hpp>
#include <gui/window.hpp>

#include <model/model.hpp>

namespace dim {
	namespace gui {
		class window;
		
		/**
		 * @brief The keyboard view handles keyboard inputs from the touch screen.
		 * It displays a keyboard as well as a text field which displays the current entry.
		 * It allows to accept an entry, updating the corresponding model, or cancel.
		 * 
		 */
		class onscreen_keyboard {
			window &m_window;
			size_t m_scene;
			std::string m_buffer;
			component *m_keys[2][2];
			uint8_t m_case;
			uint8_t m_cat;
			label *m_text_field;
			button *m_clear;
			std::vector<component*> m_components;
			model::model_value *m_value;
			model::model &m_model;
			int m_key_counter;
			uint32_t m_cursor;
			
			void shift();
			void change();
			void backspace ();
			void enter_char(const char* c);
			void enter();
			void cancel();
			void clear();
			
			std::function<void()> simple_key_func(const char* c);
			void add_key(uint32_t type, std::string text, std::function<void()>, float col, uint32_t row, float width = 1);
			void add_simple_key(uint32_t type, const char *text, float col, uint32_t row);
			
		public:
			onscreen_keyboard(window &window, model::model &model);
			~onscreen_keyboard();
			
			void show(const std::string &value_name);
		};
	}
}