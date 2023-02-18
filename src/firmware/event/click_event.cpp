#include "click_event.hpp"
#include "../gui/component.hpp"

void dim::event::click_event::handle_for_component(dim::gui::component* comp)
{
	comp->handle_click_event(*this);
}