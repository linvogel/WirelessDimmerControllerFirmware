#pragma once

#include <vector>

#include "../math/matrix.hpp"
#include "renderer.hpp"

namespace dim {
	namespace gui {
		
		class renderer; // circular dependency resolution
		class shape2;
		
		class component {
			component *m_parent;
			std::vector<component*> m_children;
			
			dim::math::vector2f m_position;
			dim::math::vector2f m_scale;
			float m_angle;
			
		protected:
			std::vector<dim::gui::shape2*> m_shapes;
			
		public:
			component(dim::math::vector2f pos, dim::math::vector2f scale, float angle);
			component(float x, float y, float sx, float sy, float angle) : component({x, y}, {sx, sy}, angle) {}
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
			
			/**
			 * @brief 
			 * 
			 * @param render 
			 */
			virtual void draw(renderer &render) final;
			
			virtual void draw_component(renderer &renderer) {};
		};
		
	}
}