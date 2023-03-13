#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "component.hpp"
#include "shape2.hpp"
#include "../math/matrix.hpp"
#include "../util/custom_array.hpp"

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace dim {
	namespace gui {
		
		class component; // circular dependency resolution
		class shape2;
		class texture;
		
		using ivec2 = dim::math::matrix<int, 1, 2>;
		
		struct glyph {
			ivec2 origin;
			ivec2 size;
			ivec2 bearing;
			int advance;
		};
		
		
		/**
		 * @brief The Renderer class contains the basic infrastructure for creating and managing windows
		 * as well as handling certain events.
		 * 
		 */
		class renderer {
		private:
			GLFWwindow *m_window;
			std::vector<matrix4f> m_proj_stack;
			std::vector<matrix4f> m_view_stack;
			std::vector<matrix4f> m_model_stack;
			std::vector<matrix4f> m_rotation_stack;
			matrix4f mvp;
			
			unsigned int m_base_program;
			unsigned int m_text_program;
			unsigned int m_texture_program;
			unsigned int m_current_program;
			
			unsigned int m_font_atlas;
			unsigned int m_text_vertex_buffer;
			int bitmap_width;
			int bitmap_height;
			std::map<char, glyph> m_font_map;
			
			FT_Library *m_ft_lib;
			std::string m_font_name;
			
			void init_text_rendering();
			
		public:
			vector4f text_color;
			renderer() = default;
			renderer(GLFWwindow *window);
			~renderer();
			
			int createShader(const std::string &vertex_shader_file, const std::string &fragment_shader_file);
			unsigned int create_2d_float_vertex_buffer_simple(size_t size, custom_array<float> &data);
			void update_buffer(unsigned int buffer, size_t size, custom_array<float> &data);
			unsigned int get_base_program() { return this->m_base_program; }
			
			unsigned int create_texture(uint8_t *data, int width, int height, int bpp, int channels);
			
			void reset();
			void wait();
			void wait(double timeout);
			void poll();
			void swap();
			void set_swap_interval(int i);
			void clear();
			
			void update_mvp();
			void reload_mvp();
			
			void push_proj();
			void pop_proj();
			void push_view();
			void pop_view();
			void push_model();
			void pop_model();
			void push_rotation();
			void pop_rotation();
			void translate(float x, float y) { this->translate({x,y}); }
			void translate(vector2f position, bool update_uniform_mvp = false);
			void rotate(float angle, bool update_uniform_mvp = false);
			void scale(float w, float h) { this->scale({w,h}); }
			void scale(vector2f scale, bool update_uniform_mvp = false);
			void transform(vector2f position, float angle, vector2f scale);
			void ortho(float left, float right, float bottom, float top, float near, float far);
			
			vector2f get_text_size(std::string text, float size);
			
			void draw_shape(shape2 *shape);
			void draw_text_centered(std::string text, float x, float y, float size);
		};
	}
}