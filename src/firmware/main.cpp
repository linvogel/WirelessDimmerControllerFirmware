#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "errors.hpp"

#include "math/matrix.hpp"

#include "gui/window.hpp"
#include "gui/button.hpp"
#include "gui/panel.hpp"
#include "gui/slider.hpp"
#include "gui/knob.hpp"
#include "gui/label.hpp"

#include "gui/onscreen_keyboard.hpp"

#define MODULE_NAME "main"
#include "logging.hpp"


using namespace dim::gui;
using namespace dim::in;
using namespace dim::model;


int main()
{
	dim::log::init();
	info("Starting DimmerControllerFirmware...");
	
	/**
	 * TODO:
	 *  - Start View Thread
	 *  - Start Controller Thread
	 *  - monitor activity
	 *  - wait for other threads
	 * 
	 * This will be the architecture later when view and controller threads are ready themselves
	 * 
	 */
	
	// create window object and prepare interfaces
	debug("Creating main window...");
	window win("Dimmer Controller", 800, 480);
	renderer &renderer = win.get_renderer();
	
	input_controller &input_controller = win.get_input_ctrl();
	
	
	debug("Setting up GUI...");

	
	debug("Setting up renderer...");
	renderer.set_swap_interval(1);
	
	info("Setup complete, entering draw loop...");
	
	while (!win.shoud_close()) {
		trace("Render cycle...");
		renderer.wait(0.25);
		//trace("Waited...");
		renderer.clear();
		//trace("Cleared...");
		
		win.draw(renderer);
		//trace("Drawn...");
		
		renderer.swap();
		//trace("Swapped...");
	}
	
	return dim::OK;
}