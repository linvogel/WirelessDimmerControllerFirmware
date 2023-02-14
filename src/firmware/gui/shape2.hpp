#pragma once

#include "../math/matrix.hpp"
#include "renderer.hpp"


const int N_VERT_CIRCLE = 32;

namespace dim {
	namespace gui {
		
		class renderer;
		
		/**
		 * @brief Base class for shapes for the renderer
		 * 
		 */
		class shape2 {
			friend renderer;
		protected:
			size_t m_nElements;
			unsigned int m_buffer;
			float* m_data;
			
			
		public:
			shape2();
			shape2(size_t size, float *positions);
			shape2(size_t size, std::initializer_list<float>);
			shape2(size_t size) : shape2(size, nullptr) {}
			
			~shape2();
			
			/**
			 * @brief Update this buffer with new data. If the size of this buffer changes the
			 * buffer will be reallocated.
			 * 
			 * @param size the number of elements in the data
			 * @param buffer the data
			 */
			virtual void update_buffer(size_t size, float* buffer);
			
			/**
			 * @brief Update this buffer under the assumption that the size has remained the same.
			 * The data has to written to the buffer prior to a call to this function. The location
			 * to write to can be queried by @link size() @endlink
			 * 
			 */
			virtual void update_buffer() { update_buffer(this->m_nElements, this->m_data); }
			
			/**
			 * @brief Get a pointer to the data. This pointer should NEVER be stored anywhere
			 * 
			 * @return float* a pointer to the data of this buffer. The size of the associated memory region
			 * can be queried with size()
			 */
			virtual float* data() { return this->m_data; }
			
			/**
			 * @brief Size of this buffer
			 * 
			 * @return size_t numer of elements in this buffer
			 */
			virtual size_t size() { return this->m_nElements; }
		};
		
		
		class line2 : public virtual shape2 {
		public:
			line2(std::initializer_list<float> values): shape2(4, values) {}
			line2(float x1, float y1, float x2, float y2);
			line2(dim::math::vector2f a, dim::math::vector2f b);
		};
		
		class triang2 : public virtual shape2 {
		public:
			triang2(std::initializer_list<float> values): shape2(6, values) {}
			triang2(float x1, float y1, float x2, float y2, float x3, float y3);
			triang2(dim::math::vector2f a, dim::math::vector2f b, dim::math::vector2f c);
		};
		
		class quad2 : public virtual shape2 {
		public:
			quad2(std::initializer_list<float> values): shape2(8, values) {}
			quad2(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
			quad2(dim::math::vector2f a, dim::math::vector2f b, dim::math::vector2f c, dim::math::vector2f d);
		};
		
		class circle2 : public virtual shape2 {
		public:
			circle2(float x, float y, float r);
			circle2(dim::math::vector2f pos, float r) : circle2(pos(0), pos(1), r) {}
		};
		
	}	
}
