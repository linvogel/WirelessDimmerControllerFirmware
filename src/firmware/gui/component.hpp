#pragma once

#include <vector>
#include <memory>

#include "../math/matrix.hpp"
#include "../event/event.hpp"
#include "../event/click_event.hpp"
#include "renderer.hpp"

namespace dim {
	namespace event {
		class event;
		class click_event;
	}
	namespace gui {
		
		class renderer; // circular dependency resolution
		class shape2;
		
		class component {
		protected:
			component *m_parent;
			std::vector<component*> m_children;
			component* m_focussed;
			
			dim::math::vector2f m_position;
			dim::math::vector2f m_size;
			dim::math::vector2f m_scale;
			float m_angle;
			
			std::shared_ptr<dim::gui::shape2> m_shape;
			
		public:
			component(dim::math::vector2f pos, dim::math::vector2f scale, float angle);
			component(float x, float y, float sx, float sy, float angle) : component({x, y}, {sx, sy}, angle) {}
			~component() = default;
			
			/**
			 * @brief Get the parent component
			 * 
			 * @return Pointer to the parent component
			 */
			virtual component* get_parent();
			
			/**
			 * @brief Get a vector of children of this component. Possibly empty.
			 * 
			 * @return a reference to the children vector.
			 */
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
			 * @brief The draw function draws this component and its children to the canvas.
			 * This includes updating the MVP matrix and restoring it once done.
			 * 
			 * @param render The renderer instance to use.
			 */
			virtual void draw(renderer &renderwe) final;
			
			/**
			 * @brief The draw component function draws this component to the canvas before its
			 * children are drawn. This function is intended to be overridden by specific components
			 * 
			 * @param renderer The renderer instance to use.
			 */
			virtual void draw_component(renderer &renderer) {};
			
			/**
			 * @brief Handles generic events sent to this component.
			 * 
			 * @param event Some event 
			 */
			virtual void handle_generic_event(std::shared_ptr<dim::event::event> event) final;
			
			/**
			 * @brief Handle a click event in this component
			 * 
			 * @param event 
			 */
			virtual void handle_click_event(dim::event::click_event event) {};
			
		};
		
	}
}