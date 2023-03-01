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
			dim::gui::component *m_comp;
			int m_keys[N_KEYS];
			bool m_buttons[N_BUTTONS];
			float m_mousex;
			float m_mousey;
			float m_dx;
			float m_dy;
			
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