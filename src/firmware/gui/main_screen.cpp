#include "main_screen.hpp"



#define MODULE_NAME "main_screen"
#include <logging.hpp>


using namespace dim::gui;


main_screen::main_screen(renderer &renderer)
	: panel(renderer, 0, 0, 800, 480), component(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 800.0f, 480.0f)
{
	// add main screen components
}