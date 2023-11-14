#pragma once

#include "component.hpp"
#include "../math/matrix.hpp"
#include "../input/input_controller.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

#include <gui/onscreen_keyboard.hpp>
#include <gui/channel_screen.hpp>

namespace dim {
	namespace model {
		class model;
	};
	namespace gui {
		class onscreen_keyboard;
		class channel_screen;
		
		/**
		 * @brief Unique window class to be used in this firmware. Only a single instance of this window may be created.
		 * 
		 */
		class window : public virtual component {
		private:
			GLFWwindow *m_window;
			std::shared_ptr<renderer> m_renderer;
			std::shared_ptr<dim::in::input_controller> m_input_ctrl;
			std::vector<std::vector<dim::gui::component*>> m_scenes;
			size_t m_scene;
			std::vector<size_t> m_scene_stack;
			
			onscreen_keyboard *m_onscreen_kbd;
			channel_screen *m_channel_screen;
			
		public:
			/**
			 * @brief Construct a new window object. This instance must be the only one in this process!
			 * 
			 * @param name Name of the window. This is not really important since it is not actualy displayed on the system.
			 * @param width Width of the window.
			 * @param height Height of the window.
			 */
			window(std::string name, int width, int height);
			~window();
			
			virtual void draw_component(renderer &renderer);
			
			GLFWwindow *get_window() { return this->m_window; };
			renderer &get_renderer() { return *(this->m_renderer); };
			dim::in::input_controller &get_input_ctrl() { return *(this->m_input_ctrl); };
			int shoud_close();
			
			// scene related functions and overrides
			virtual void add(dim::gui::component *child) override;
			virtual void remove_child(dim::gui::component *child) override;
			void set_scene(size_t scene);
			size_t get_scene() { return this->m_scene; };
			size_t create_scene();
			
			void push_scene(size_t scene);
			void pop_scene();
			
			void set_keyboard(onscreen_keyboard *okbd);
			void show_keyboard(const std::string &value_name);
			
			void set_channel_screen(channel_screen *ch_screen);
			void show_channel_screen(const std::string &channel_prefix);
		};
		
	}
}