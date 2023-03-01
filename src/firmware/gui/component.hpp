#pragma once

#include <vector>
#include <memory>

#include <iostream>

#include "../math/matrix.hpp"
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
			
			vector2f m_position;
			vector2f m_size;
			vector2f m_scale;
			float m_angle;
			
			std::shared_ptr<dim::gui::shape2> m_shape;
			
			vector4f m_bg_color;
			vector4f m_fg_color;
			
		public:
			component(vector2f pos, vector2f scale, float angle, vector2f size);
			component(float x, float y, float scalex, float scaley, float angle, float sizex, float sizey) : component({x, y}, {scalex, scaley}, angle, {sizex, sizey}) {}
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
			virtual void draw_component(renderer &renderer);
			
			virtual void set_shape(std::shared_ptr<shape2> shape) { this->m_shape = shape; }
			virtual std::shared_ptr<shape2> get_shape() { return this->m_shape; }
			
			virtual component* get_focussed() { return this->m_focussed; }
			virtual void focus(component* comp) { this->m_focussed = comp; }
			virtual bool hit(float local_x, float local_y); 
			virtual component* hit_children(float local_x, float local_y); 
			
			
			virtual void onMouseEnter() {};
			virtual void onMouseExit() {};
			virtual void onLeftMouseDown(float local_x, float local_y) {}
			virtual void onLeftMouseUp(float local_x, float local_y) {}
			virtual void onRightMouseDown(float local_x, float local_y) {}
			virtual void onRightMouseUp(float local_x, float local_y) {}
			virtual void onKeyPressed(int key, int mod) {}
			virtual void onKeyReleased(int key, int mod) {}
			virtual void onKeyTyped(int key, int mod) {}
			virtual void onScrollUp() {}
			virtual void onScrollDown() {}
			virtual void onScrollLeft() {}
			virtual void onScrollRight() {}
		};
		
	}
}