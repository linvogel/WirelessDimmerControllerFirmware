#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MODULE_NAME "main"
#include "logging.hpp"
#include "errors.hpp"

#include "math/matrix.hpp"

#include "gui/window.hpp"
#include "gui/button.hpp"
#include "gui/panel.hpp"
#include "gui/slider.hpp"

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
	
	vector4f cols[5] = {
		{0.0f, 0.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},	
		{1.0f, 1.0f, 1.0f, 1.0f}
	};
	int count = 0;
	
	button btn1("Button", [&]() { renderer.text_color = cols[count++ % 5]; }, renderer, 10, 10, 150, 30);
	button btn2("Mute", []() { std::cout << "btn2 pressed" << std::endl; }, renderer, 10, 50, 150, 30);
	
	slider sld1(renderer, 300, 100, 60, 280, 0, 1);
	
	panel pnl1(renderer, 100, 100, 170, 90);
	
	window.add(&pnl1);
	window.add(&sld1);
	pnl1.add(&btn1);
	pnl1.add(&btn2);
	
	renderer.set_swap_interval(1);
	
	while (!window.shoud_close()) {
		//trace("Render cycle...");
		renderer.wait(0.25);
		//trace("Waited...");
		renderer.clear();
		//trace("Cleared...");
		
		window.draw(renderer);
		//trace("Drawn...");
		
		renderer.swap();
		//trace("Swapped...");
	}
	
	return dim::OK;
}