#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../logging.hpp"
#include "../math/matrix.hpp"


namespace dim {
	namespace gui {
		/**
		 * @brief The Renderer class contains the basic infrastructure for creating and managing windows
		 * as well as handling certain events.
		 * 
		 */
		class renderer {
		private:
			GLFWwindow *m_window;
			
		public:
			renderer(GLFWwindow *window);
			~renderer();
			
			void wait();
			void wait(double timeout);
			void poll();
			void swap();
			void set_swap_interval(int i);
			
		};
	}
}