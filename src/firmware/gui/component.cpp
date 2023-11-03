#include "component.hpp"
#include <algorithm>

#define MODULE_NAME "component"
#include "../logging.hpp"

using namespace dim::gui;
using namespace dim::math;

component::component(vector2f pos, vector2f scale, float angle, vector2f size)
{
	ftrace();
	this->m_focussed = nullptr;
	this->m_parent = nullptr;
	this->m_position = pos;
	this->m_scale = scale;
	this->m_angle = angle;
	this->m_size = size;
	
	// TODO: this should be configurable
	this->m_bg_color = vector4f({0.0f, 0.145f, 0.274f, 1.0f});
	this->m_fg_color = vector4f({0.7f, 0.7f, 0.8f, 1.0f});
}

component* component::get_parent()
{
	ftrace();
	return this->m_parent;
}

const std::vector<component*>& component::get_children()
{
	ftrace();
	return this->m_children;
}

void component::add(component *comp)
{
	ftrace();
	comp->m_parent = this;
	this->m_children.push_back(comp);
	if (comp->m_shape.get()) {
		comp->m_shape->set_offset(this->m_shape->get_offset()(0) + comp->m_position(0), this->m_shape->get_offset()(1) + comp->m_position(1) - this->m_size(1) + comp->m_size(1));
	}
}

void component::remove_child(component *comp)
{
	ftrace();
	// go through all children backwards and remove any that are equal to the input.
	for (int64_t i = this->m_children.size()-1; i >= 0; i--) {
		if (this->m_children[i] == comp) {
			this->m_children.erase(this->m_children.begin()+i);
		}
	}
}

void component::draw_component(renderer &renderer)
{
	ftrace();
	if (this->m_shape.get()) renderer.draw_shape(this->m_shape.get());
}

void component::draw(renderer &renderer)
{
	ftrace();
	renderer.push_proj();
	renderer.transform(this->m_position, this->m_angle, this->m_scale);
	this->draw_component(renderer);
	for (component* comp : this->m_children) {
		comp->draw(renderer);
	}
	
	renderer.pop_proj();
}

component* component::hit_children(float local_x, float local_y)
{
	ftrace();
	for (component* comp : this->m_children) {
		// TODO: scale and rotate local point
		if (comp && comp->hit(local_x, local_y)) return comp->hit_children(local_x - comp->m_position(0), local_y - comp->m_position(1));
	}
	
	return this;
}

bool component::hit(float local_x, float local_y)
{
	ftrace();
	return	this->m_position(0) <= local_x &&
			this->m_position(1) <= local_y &&
			this->m_position(0) + this->m_size(0) >= local_x &&
			this->m_position(1) + this->m_size(1) >= local_y;
}