#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION // This can be done here, since this is the only place where this library is used (for now at least)
#include <stb_image.h>

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif
#define MODULE_NAME "texture"
#include "../logging.hpp"

using namespace dim::gui;

texture::texture(renderer &renderer, std::string texture_name)
{
	debug("Creating texture: %s", texture_name.c_str());
	int n_width, n_height, n_bpp;
	uint8_t *data = stbi_load(("textures/" + texture_name + "_base.png").c_str(), &(this->m_width), &(this->m_height), &(this->m_bpp), 4);
	this->m_base_texture = renderer.create_texture(data, this->m_width, this->m_height, this->m_bpp, 4);
	stbi_image_free(data);
	
	data = stbi_load(("textures/" + texture_name + "_normal.png").c_str(), &n_width, &n_height, &n_bpp, 4);
	if (this->m_width != n_width || this->m_height != n_height)
		warn("Texture loading issue: base texture and normal map have different sizes!");
	this->m_normal_texture = renderer.create_texture(data, n_width, n_height, n_bpp, 4);
	stbi_image_free(data);
	
	// TODO: switch to an atlas model
	this->m_uv[0] = {0.0f, 0.0f};
	this->m_uv[1] = {1.0f, 0.0f};
	this->m_uv[2] = {1.0f, 1.0f};
	this->m_uv[3] = {0.0f, 1.0f};
	
}