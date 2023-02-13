#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logging.hpp"
#include "errors.hpp"

#include "math/matrix.hpp"

#include "gui/window.hpp"


int main()
{
	dim::log::init();
	info("Starting DimmerControllerFirmware...");
	
	dim::gui::window window("Dimmer Controller", 800, 480);
	dim::gui::renderer renderer(window.get_window());
	
	renderer.set_swap_interval(1);
	
	while (!window.shoud_close()) {
		renderer.wait();
		
		window.draw(renderer);
		
		renderer.swap();
	}
	
	return dim::OK;
}