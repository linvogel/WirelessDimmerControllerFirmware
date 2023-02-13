#include "component.hpp"
#include <algorithm>

dim::gui::component* dim::gui::component::get_parent()
{
	return this->m_parent;
}

const std::vector<dim::gui::component*>& dim::gui::component::get_children()
{
	return this->m_children;
}

void dim::gui::component::add(dim::gui::component *comp)
{
	this->m_children.push_back(comp);
}

void dim::gui::component::remove_child(dim::gui::component *comp)
{
	// go through all children backwards and remove any that are equal to the input.
	for (int64_t i = this->m_children.size()-1; i >= 0; i--) {
		if (this->m_children[i] == comp) {
			this->m_children.erase(this->m_children.begin()+i);
		}
	}
}

void dim::gui::component::draw(dim::gui::renderer &renderer)
{
	this->draw_component(renderer);
	
	for (dim::gui::component* comp : this->m_children) {
		comp->draw(renderer);
	}
}

