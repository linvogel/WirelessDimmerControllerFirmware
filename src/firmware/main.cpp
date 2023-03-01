#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MODULE_NAME "main"
#include "logging.hpp"
#include "errors.hpp"

#include "math/matrix.hpp"

#include "gui/window.hpp"
#include "gui/button.hpp"

using namespace dim::gui;
using namespace dim::in;


int main()
{
	dim::log::init();
	info("Starting DimmerControllerFirmware...");
	
	/**
	 * TODO:
	 *  - Start model thread
	 *  - Start Interface Thread
	 *  - Start View Thread
	 *  - monitor activity
	 *  - wait for other threads
	 * 
	 * This will be the architecture later when model, view and interface threads are ready themselves
	 * 
	 */
	
	// create window object and prepare interfaces
	window window("Dimmer Controller", 800, 480);
	renderer &renderer = window.get_renderer();
	input_controller &input_controller = window.get_input_ctrl();
	
	button btn1("Button", []() {std::cout << "btn1 pressed" << std::endl;}, renderer, 100, 100, 150, 30);
	
	window.add(&btn1);
	
	renderer.set_swap_interval(1);
	
	while (!window.shoud_close()) {
		trace("Render cycle...");
		renderer.wait(0.25);
		trace("Waited...");
		renderer.clear();
		trace("Cleared...");
		
		window.draw(renderer);
		trace("Drawn...");
		
		renderer.swap();
		trace("Swapped...");
	}
	
	return dim::OK;
}