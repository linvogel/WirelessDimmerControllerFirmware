#include "event.hpp"

using namespace dim::event;
using namespace dim::math;
using namespace dim::gui;

event::event(vector2f location)
{
	this->m_location = location;
}

event::~event()
{
	
}

vector2f& event::get_location()
{
	return this->m_location;
}

void event::handle_for_component(component *comp)
{
	warn("Handling generic event for component...");
}