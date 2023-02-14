#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "component.hpp"
#include "shape2.hpp"
#include "../logging.hpp"
#include "../math/matrix.hpp"


namespace dim {
	namespace gui {
		
		class component; // circular dependency resolution
		class shape2;
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
			
			void push();
			void pop();
			void transform(dim::math::vector2f position, float angle, dim::math::vector2f scale);
			
			void draw_shape(shape2 *shape);
			
		};
	}
}