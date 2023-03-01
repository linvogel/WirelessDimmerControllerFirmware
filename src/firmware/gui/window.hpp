#pragma once

#include "component.hpp"
#include "../math/matrix.hpp"
#include "../input/input_controller.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

namespace dim {
	namespace gui {
		
		/**
		 * @brief Unique window class to be used in this firmware. Only a single instance of this window may be created.
		 * 
		 */
		class window : public virtual component {
		private:
			GLFWwindow *m_window;
			std::shared_ptr<renderer> m_renderer;
			std::shared_ptr<dim::in::input_controller> m_input_ctrl;
			
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
			
		};
		
	}
}