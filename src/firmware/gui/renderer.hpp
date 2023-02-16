#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "component.hpp"
#include "shape2.hpp"
#include "../logging.hpp"
#include "../math/matrix.hpp"


namespace dim {
	namespace gui {
		
		class component; // circular dependency resolution
		class shape2;
		/**
		 * @brief The Renderer class contains the basic infrastructure for creating and managing windows
		 * as well as handling certain events.
		 * 
		 */
		class renderer {
		private:
			GLFWwindow *m_window;
			std::vector<dim::math::matrix4f> m_proj_stack;
			std::vector<dim::math::matrix4f> m_view_stack;
			std::vector<dim::math::matrix4f> m_model_stack;
			
			unsigned int m_base_program;
			unsigned int m_current_program;
			
			void update_mvp();
			
		public:
			renderer() = default;
			renderer(GLFWwindow *window);
			~renderer();
			
			int createShader(const std::string &vertex_shader_file, const std::string &fragment_shader_file);
			unsigned int create_2d_float_vertex_buffer_simple(size_t size, float* data);
			void update_buffer(unsigned int buffer, size_t size, float* data);
			
			void reset();
			void wait();
			void wait(double timeout);
			void poll();
			void swap();
			void set_swap_interval(int i);
			void clear();
			
			void push_proj();
			void pop_proj();
			void push_view();
			void pop_view();
			void push_model();
			void pop_model();
			void translate(float x, float y) { this->translate({x,y}); }
			void translate(dim::math::vector2f position, bool update_uniform_mvp = false);
			void rotate(float angle, bool update_uniform_mvp = false);
			void scale(float w, float h) { this->scale({w,h}); }
			void scale(dim::math::vector2f scale, bool update_uniform_mvp = false);
			void transform(dim::math::vector2f position, float angle, dim::math::vector2f scale);
			void ortho(float left, float right, float bottom, float top, float near, float far);
			
			void draw_shape(shape2 *shape);
			
		};
	}
}