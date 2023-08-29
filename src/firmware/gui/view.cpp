#include "view.hpp"


using namespace dim::gui;

view::view(window *window)
{
	this->m_window = window;
	this->m_scene = this->m_window->create_scene();
	this->m_window->set_scene(this->m_scene);
}

void view::show()
{
	this->m_window->set_scene(this->m_scene);
}