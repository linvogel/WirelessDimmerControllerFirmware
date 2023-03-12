#pragma once

#include <string>
#include <memory>

#include "renderer.hpp"

namespace dim {
	namespace gui {
		
		class renderer;
		class textured_quad2;
		
		class texture {
			friend renderer;
			friend textured_quad2;
		private:
			int m_width;
			int m_height;
			int m_bpp;
			
			vector2f m_uv[4];
			
			unsigned int m_base_texture;
			unsigned int m_normal_texture;
			
		public:
			texture(renderer &renderer, std::string filename);
			
			
		};
		
	}
}
