#pragma once

#include "../math/matrix.hpp"
#include "renderer.hpp"
#include "texture.hpp"

#include "../util/custom_array.hpp"

const int N_VERT_CIRCLE = 32;

namespace dim {
	namespace gui {
		
		class renderer;
		class texture;
		
		struct bounds {
			float x;
			float y;
			float w;
			float h;
			
			bounds() : x(0), y(0), w(1), h(1) {}
			bounds(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
		};
		
		/**
		 * @brief Base class for shapes for the renderer
		 * 
		 */
		class shape2 {
			friend renderer;
		protected:
			size_t m_nElements;
			unsigned int m_buffer;
			custom_array<float> m_data;
			
			vector4f m_offset;
			bounds m_bounds;
			float m_corner_radius;
			float m_edge_smoothness;
			float m_stroke_weight;
			vector4f m_stroke_color;
			vector4f m_background_color;
			
			unsigned int m_special_program;
			
		public:
			shape2(renderer &renderer, size_t size, float *positions);
			shape2(renderer &renderer, size_t size, std::initializer_list<float>);
			shape2(renderer &renderer, size_t size) : shape2(renderer, size, nullptr) {}
			
			~shape2();
			
			/**
			 * @brief Update this buffer with new data. If the size of this buffer changes the
			 * buffer will be reallocated.
			 * 
			 * @param size the number of elements in the data
			 * @param buffer the data
			 */
			virtual void update_buffer(renderer &renderer, size_t size, custom_array<float> &buffer);
			
			/**
			 * @brief Update this buffer under the assumption that the size has remained the same.
			 * The data has to written to the buffer prior to a call to this function. The location
			 * to write to can be queried by @link size() @endlink
			 * 
			 */
			virtual void update_buffer(renderer &renderer) { update_buffer(renderer, this->m_nElements, this->m_data); }
			
			/**
			 * @brief Get a pointer to the data. This pointer should NEVER be stored anywhere
			 * 
			 * @return float* a pointer to the data of this buffer. The size of the associated memory region
			 * can be queried with size()
			 */
			virtual custom_array<float> data() { return this->m_data; }
			
			/**
			 * @brief Size of this buffer
			 * 
			 * @return size_t numer of elements in this buffer
			 */
			virtual size_t size() { return this->m_nElements; }
			
			/**
			 * @brief Set the corner radius of this shape. This is only recommended
			 * for axis aligned quads for the time being.
			 * 
			 * @param corner_radius the corner radius in pixels
			 */
			virtual void set_corner_radius(float corner_radius) { this->m_corner_radius = corner_radius; }
			
			/**
			 * @brief Set the stroke weight of this shape. This is only recommended
			 * for axis aligned quads for the time being.
			 * 
			 * @param stroke_weight the stroke weight in pixels
			 */
			virtual void set_stroke_weight(float stroke_weight) { this->m_stroke_weight = stroke_weight; }
			
			/**
			 * @brief Set the stroke color of this shape. This is only recommended
			 * for axis aligned quads for the time being.
			 *
			 * @param stroke_color the stroke color as RGBA
			 */
			virtual void set_stroke_color(vector4f stroke_color) { this->m_stroke_color = stroke_color; }
			
			/**
			 * @brief Set the background color of this shape.
			 * 
			 * @param bg_color the background color as RGBA
			 */
			virtual void set_background_color(vector4f bg_color) { this->m_background_color = bg_color; }
			
			/**
			 * @brief Get the bounds of this shape
			 * 
			 * @return vector4f containing the bounds of this shape
			 */
			virtual vector4f get_bounds() final { return vector4f({this->m_bounds.x, this->m_bounds.y, this->m_bounds.w, this->m_bounds.h}); }
			
			/**
			 * @brief Get the size of this shape
			 * 
			 * @return vector2f containing the size of this shape
			 */
			virtual vector2f get_size() final { return vector2f({this->m_bounds.w, this->m_bounds.h}); }
			
			virtual void set_special_program(unsigned int program) { this->m_special_program = program; }
			
			virtual void set_offset(float x, float y) { this->m_offset(0) = x; this->m_offset(1) = y; }
			virtual vector4f get_offset() { return this->m_offset; }
			
			virtual bool is_textured() { return false; }
		};
		
		
		class line2 : public virtual shape2 {
		public:
			line2(renderer &renderer, std::initializer_list<float> values): shape2(renderer, 4, values) {}
			line2(renderer &renderer, float x1, float y1, float x2, float y2);
			line2(renderer &renderer, vector2f a, vector2f b);
		};
		
		class triang2 : public virtual shape2 {
		public:
			triang2(renderer &renderer, std::initializer_list<float> values): shape2(renderer, 6, values) {}
			triang2(renderer &renderer, float x1, float y1, float x2, float y2, float x3, float y3);
			triang2(renderer &renderer, vector2f a, vector2f b, vector2f c);
		};
		
		class quad2 : public virtual shape2 {
		public:
			quad2(renderer &renderer, std::initializer_list<float> values): shape2(renderer, 8, values) {}
			quad2(renderer &renderer, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
			quad2(renderer &renderer, vector2f a, vector2f b, vector2f c, vector2f d);
		};
		
		class circle2 : public virtual shape2 {
		public:
			circle2(renderer &renderer, float x, float y, float r);
			circle2(renderer &renderer, vector2f pos, float r) : circle2(renderer, pos(0), pos(1), r) {}
			
			void set_corner_radius(float rad) override { this->m_corner_radius = 0; }
		};
		
		class textured_quad2 : public virtual shape2 {
			friend renderer;
			
			texture *m_texture;
			
		public:
			textured_quad2(renderer &renderer, std::initializer_list<float> values);
			textured_quad2(renderer &renderer, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
			textured_quad2(renderer &renderer, vector2f a, vector2f b, vector2f c, vector2f d);
			
			~textured_quad2();
			
			void set_texture(texture *texture);
			void update_uv();
			
			virtual bool is_textured() override { return true; }
		};
		
	}
}
