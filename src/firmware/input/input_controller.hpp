#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const static int N_KEYS = 256;
const static int N_BUTTONS = 64;

namespace dim {
	namespace in {
		
		class input_controller {
		private:
			GLFWwindow *m_window;
			int m_keys[N_KEYS];
			int m_buttons[N_BUTTONS];
			double m_mousex;
			double m_mousey;
		public:
			input_controller();
			input_controller(GLFWwindow* window);
			~input_controller();
			
			void key_callback(int key, int scancode, int action, int mods);
			void mouse_callback(double x, double y);
		};
		
	}
}