#pragma once

#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include "../logging.hpp"


namespace dimmer {
	/**
	 * @brief The Renderer class contains the basic infrastructure for creating and managing windows
	 * as well as handling certain events.
	 * 
	 */
	class renderer {
	private:
		GLFWwindow *m_window;
		
	public:
		renderer();
		~renderer();
		
		
	};
}