#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../gui/component.hpp"

const static int N_KEYS = 256;
const static int N_BUTTONS = 64;

namespace dim {
	namespace in {
		
		class input_controller {
		private:
			GLFWwindow *m_window;
			int m_keys[N_KEYS];
			int m_buttons[N_BUTTONS];
			float m_mousex;
			float m_mousey;
			
			dim::gui::component* m_comp;
			
		public:
			input_controller();
			input_controller(GLFWwindow *window, dim::gui::component *comp);
			~input_controller();
			
			void key_callback(int key, int scancode, int action, int mods);
			void mouse_callback(double x, double y);
			void mouse_button_callback(int button, int action, int mods);
		};
		
	}
}