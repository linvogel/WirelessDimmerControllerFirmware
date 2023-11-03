#include <iostream>
#include <fstream>

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
#include "gui/channel_screen.hpp"

#include "shell/shell.hpp"

#define MODULE_NAME "main"
#include "logging.hpp"


using namespace dim::gui;
using namespace dim::in;
using namespace dim::model;
using namespace dim::sh;

void load_config(model &mod)
{
	ftrace();
	info("Loading configuration...");
	// first load configuration file
	mod.load_category("cfg", "config.cfg");
	
	// determine language selected by the configuration
	verbose("Detecting language selection...");
	if (!mod.contains_key("lang")) {
		warn("Configuration does not contain language selection. Defaulting to english...");
		mod.add("lang", "english");
	}
	trace("Loading language file...");
	std::string lang_filename = std::string("lang/") + static_cast<std::string>(mod["lang"]) + std::string(".cfg");
	mod.load_category("language", lang_filename);
	
	// TODO: load device state file
	
	info("Config ready!");
}

void save_config(model &mod)
{
	ftrace();
	info("Saving configuration...");
	
	// TODO: device state file
	
	// determine language and save language file
	verbose("Saving language...");
	std::string lang_filename = std::string("lang/") + static_cast<std::string>(mod["lang"]) + std::string(".cfg");
	mod.save_category("language", lang_filename);
	
	// save main config file
	mod.save_category("cfg", "config.cfg");
	
	info("Configuration saved!");
}

int main()
{
	ftrace();
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
	
	// load the configuration
	model mod;
	load_config(mod);
	
	// create window object and prepare interfaces
	debug("Creating main window...");
	window win("Dimmer Controller", 800, 480);
	renderer &renderer = win.get_renderer();
	onscreen_keyboard okbd(win, mod);
	channel_screen ch_screen(renderer, win, mod);
	
	win.set_keyboard(&okbd);
	win.set_channel_screen(&ch_screen);
	
	input_controller &input_controller = win.get_input_ctrl();
	
	
	debug("Setting up GUI...");
	
	debug("Setting up renderer...");
	renderer.set_swap_interval(1);
	
	// save the config again, to store any defaults that got inserted
	save_config(mod);
	
	// setting up shell
	debug("Setting up shell...");
	shell shell(win);
	
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