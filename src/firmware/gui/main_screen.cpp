#include "main_screen.hpp"



#define MODULE_NAME "main_screen"
#include <logging.hpp>


using namespace dim::gui;
using namespace dim::model;


main_screen::main_screen(window &window, renderer &renderer, dim::model::model &model)
	: m_window(window)
	, m_renderer(renderer)
	, m_model(model)
{
	// add main screen components
	this->m_scene = this->m_window.create_scene();
	this->m_window.set_scene(this->m_scene);
}