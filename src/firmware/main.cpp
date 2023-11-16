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
	debug("Detecting language selection...");
	if (!mod.contains_key("lang")) {
		warn("Configuration does not contain language selection. Defaulting to english...");
		mod.add("lang", "english");
	}
	debug("Loading language file...");
	std::string lang_filename = std::string("lang/") + static_cast<std::string>(mod["lang"]) + std::string(".cfg");
	mod.load_category("language", lang_filename);
	
	// load scene file
	debug("Detecting scene selection...");
	if (!mod.contains_key("scene")) {
		warn("Configuration does not contain scene selection. Defaulting to scene0...");
		mod.add("scene", "scene0");
	}
	debug("Loading scene file...");
	std::string scene_filename = std::string("scene/") + static_cast<std::string>(mod["scene"]) + std::string(".cfg");
	mod.load_category("scene", scene_filename);
	
	info("Config ready!");
}

void save_config(model &mod)
{
	ftrace();
	info("Saving configuration...");
	
	// TODO: device state file
	
	// determine language and save language file
	debug("Saving language...");
	std::string lang_filename = std::string("lang/") + static_cast<std::string>(mod["lang"]) + std::string(".cfg");
	mod.save_category("language", lang_filename);
	
	// save main config file
	mod.save_category("cfg", "config.cfg");
	
	info("Configuration saved!");
}

int main()
{
	dim::log::init();
	dim::log::set_log_level(dim::log::VERBOSE);
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
	debug("Setting up GUI...");
	debug("Creating main window...");
	window win("Dimmer Controller", 800, 480);
	renderer &renderer = win.get_renderer();
	
	debug("Creating screens...");
	onscreen_keyboard okbd(win, mod);
	channel_screen ch_screen(renderer, win, mod);
	
	debug("Attaching screens to window");
	win.set_keyboard(&okbd);
	win.set_channel_screen(&ch_screen);
	
	win.show_channel_screen("scene.channel_0");
	
	input_controller &input_controller = win.get_input_ctrl();
	
	
	
	debug("Setting up renderer...");
	renderer.set_swap_interval(1);
	
	// save the config again, to store any defaults that got inserted
	save_config(mod);
	
	// setting up shell
	debug("Setting up shell...");
	shell shell(win);
	
	info("Setup complete, entering draw loop...");
	
	while (!win.shoud_close()) {
		verbose("Render cycle...");
		renderer.clear_frame_data();
		renderer.set_frame_data("focussed", win.get_focussed());
		
		renderer.wait(0.25);
		//verbose("Waited...");
		renderer.clear();
		//verbose("Cleared...");
		
		win.draw(renderer);
		//verbose("Drawn...");
		
		renderer.swap();
		//verbose("Swapped...");
	}
	
	return dim::OK;
}