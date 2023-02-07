#include "component.hpp"
#include <algorithm>

dimmer::gui::component* dimmer::gui::component::get_parent()
{
	return this->m_parent;
}

const std::vector<dimmer::gui::component*>& dimmer::gui::component::get_children()
{
	return this->m_children;
}

void dimmer::gui::component::add(dimmer::gui::component *comp)
{
	this->m_children.push_back(comp);
}

void dimmer::gui::component::remove_child(dimmer::gui::component *comp)
{
	// go through all children backwards and remove any that are equal to the input.
	for (int64_t i = this->m_children.size()-1; i >= 0; i--) {
		if (this->m_children[i] == comp) {
			this->m_children.erase(this->m_children.begin()+i);
		}
	}
}

