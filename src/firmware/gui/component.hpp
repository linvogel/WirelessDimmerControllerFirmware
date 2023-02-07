#pragma once

#include <vector>

#include "renderer.hpp"

namespace dimmer {
	namespace gui {
		
		class component {
			component *m_parent;
			std::vector<component*> m_children;
			
		public:
			component() = default;
			~component() = default;
			
			virtual component* get_parent();
			virtual const std::vector<component*>& get_children();
			
			/**
			 * @brief Add new component as a child to this component.
			 * Any operations on the new child component will now be relative to the position and rotation
			 * of this component.
			 * 
			 * @param comp 
			 */
			virtual void add(component *comp);
			
			/**
			 * @brief Remove child component from the list of children of this component.
			 * This function is expensive in terms of worst-case runtime and is not meant to be called
			 * repeatedly.
			 * 
			 * @param comp Value to be removed from the list of children of this component.
			 */
			virtual void remove_child(component *comp);
			
			virtual void draw(renderer &render);
		};
		
	}
}