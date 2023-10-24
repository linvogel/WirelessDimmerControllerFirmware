#include "shape2.hpp"

#include <GL/glew.h>
#include <memory>
#include <cstring>
#include <cmath>

#define MODULE_NAME "shape2"
#include "../logging.hpp"

using namespace dim::gui;

shape2::shape2(renderer &renderer, size_t size, float *data)
	: m_corner_radius(0)
	, m_edge_smoothness(1)
	, m_special_program(renderer.get_base_program())
	, m_stroke_weight(0)
{
	debug("creating shape2...");
	this->m_buffer = 0;
	this->m_nElements = size;
	this->m_data = custom_array<float>(this->m_nElements);
	
	verbose("shape2 buffer allocated, copying supplied data...");
	
	if (data) for (size_t i = 0; i < this->m_nElements; i++) this->m_data[i] = data[i];
	
	this->m_buffer = renderer.create_2d_float_vertex_buffer_simple(this->m_nElements, this->m_data);
	
	this->m_offset = {0, 0, 0, 0};
	
	// Note: this only works for 2d vertex buffer with only the position attribute
	this->m_bounds = {800, 480, 0, 0};
	for (size_t i = 0; i < this->m_nElements-1; i+= 2) {
		this->m_bounds.x = std::min(this->m_bounds.x, this->m_data[i]);
		this->m_bounds.y = std::min(this->m_bounds.y, this->m_data[i+1]);
		this->m_bounds.w = std::max(this->m_bounds.w, this->m_data[i]);
		this->m_bounds.h = std::max(this->m_bounds.h, this->m_data[i+1]);
	}
	this->m_bounds.w -= this->m_bounds.x;
	this->m_bounds.h -= this->m_bounds.y;
	
	this->m_stroke_color = {0.0f, 0.0f, 1.0f, 1.0f};
	this->m_background_color = {0.0f, 0.0f, 0.0f, 0.0f};
	
	verbose("shape2 created");
}

shape2::shape2(renderer &renderer, size_t size, std::initializer_list<float> values) : shape2(renderer, size)
{
	if (size != values.size()) {
		warn("shape2 init received initializer list that is not the same size as the declared buffer!");
	}
	
	size_t i = 0;
	for (float f : values) {
		if (i == std::min(values.size(), size)) break;
		this->m_data[i++] = f;
	}
	
	// update the buffer so that the data travels to the gpu
	this->update_buffer(renderer);
}

shape2::~shape2()
{
	debug("destroying shape2");
}

void shape2::update_buffer(renderer &renderer, size_t size, custom_array<float> &data)
{
	this->m_data = data;
	if (this->m_data.get() == nullptr) return;
	
	renderer.update_buffer(this->m_buffer, this->m_nElements, this->m_data);
	
	this->m_bounds = {800, 480, 0, 0};
	for (size_t i = 0; i < this->m_nElements; i+= 2) {
		float a = this->m_data[i];
		this->m_bounds.x = std::min(this->m_bounds.x, this->m_data[i]);
		this->m_bounds.y = std::min(this->m_bounds.y, this->m_data[i+1]);
		this->m_bounds.w = std::max(this->m_bounds.w, this->m_data[i]);
		this->m_bounds.h = std::max(this->m_bounds.h, this->m_data[i+1]);
	}
	this->m_bounds.w -= this->m_bounds.x;
	this->m_bounds.h -= this->m_bounds.y;
}


// line 2
line2::line2(renderer &renderer, float x1, float y1, float x2, float y2)
	: shape2(renderer, 4, {x1, y1, x2, y2}) {}
line2::line2(renderer &renderer, vector2f a, vector2f b)
	: shape2(renderer, 4, {a(0), a(1), b(0), b(1)}) {}

// triang 2
triang2::triang2(renderer &renderer, float x1, float y1, float x2, float y2, float x3, float y3)
	: shape2(renderer, 6, {x1, y1, x2, y2, x3, y3}) {}
triang2::triang2(renderer &renderer, vector2f a, vector2f b, vector2f c)
	: shape2(renderer, 6, { a(0), a(1), b(0), b(1), c(0), c(1) }) {}

// quad 2
quad2::quad2(renderer &renderer, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
	: shape2(renderer, 12, {x1, y1, x2, y2, x3, y3, x1, y1, x3, y3, x4, y4}) {
}
quad2::quad2(renderer &renderer, vector2f a, vector2f b, vector2f c, vector2f d)
	: shape2(renderer, 12, { a(0), a(1), b(0), b(1), c(0), c(1), a(0), a(1), c(0), c(1), d(0), d(1) }) {
}

// circle 2
circle2::circle2(renderer &renderer, float x, float y, float r) : shape2(renderer, 2*N_VERT_CIRCLE + 4)
{
	// prepare some space for the values
	vector2f current({1, 0});
	vector2f tmp;
	// prepare for geometric calculations
	double angle = PI2 / N_VERT_CIRCLE;
	float s = static_cast<float>(std::sin(angle));
	float c = static_cast<float>(std::cos(angle));
	matrix2f rot_mat({c, -s, s, c});
	
	this->m_data[0] = x;
	this->m_data[1] = y;
	
	// rotate current and push scaled copies of it into storage
	for (int i = 1; i <= N_VERT_CIRCLE; i++) {
		tmp = current * r;
		current = rot_mat * current;
		this->m_data[2*i + 0] = x + tmp(0);
		this->m_data[2*i + 1] = y + tmp(1);
	}
	
	// duplicate the last point to close the circle
	this->m_data[2*N_VERT_CIRCLE + 2] = this->m_data[2];
	this->m_data[2*N_VERT_CIRCLE + 3] = this->m_data[3];
	
	this->update_buffer(renderer);
}

// textured quad2
textured_quad2::textured_quad2(renderer &renderer, std::initializer_list<float> values)
	: shape2(renderer, 6 * 4)
{
	this->m_texture =  nullptr;
	// order compatible with the generic quad2 implementation: 1 2 3 1 3 4
	this->m_data[0] = *(values.begin()+0);
	this->m_data[1] = *(values.begin()+1);
	this->m_data[2] = 0;
	this->m_data[3] = 0;
	this->m_data[4] = *(values.begin()+2);
	this->m_data[5] = *(values.begin()+3);
	this->m_data[6] = 0;
	this->m_data[7] = 0;
	this->m_data[8] = *(values.begin()+4);
	this->m_data[9] = *(values.begin()+5);
	this->m_data[10] = 0;
	this->m_data[11] = 0;
	this->m_data[12] = *(values.begin()+0);
	this->m_data[13] = *(values.begin()+1);
	this->m_data[14] = 0;
	this->m_data[15] = 0;
	this->m_data[16] = *(values.begin()+4);
	this->m_data[17] = *(values.begin()+5);
	this->m_data[18] = 0;
	this->m_data[19] = 0;
	this->m_data[20] = *(values.begin()+6);
	this->m_data[21] = *(values.begin()+7);
	this->m_data[22] = 0;
	this->m_data[23] = 0;
}

textured_quad2::textured_quad2(renderer &renderer, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
	: textured_quad2(renderer, {x1, y1, x2, y2, x3, y3, x4, y4}) {}
textured_quad2::textured_quad2(renderer &renderer, vector2f a, vector2f b, vector2f c, vector2f d)
	: textured_quad2(renderer, { a(0), a(1), b(0), b(1), c(0), c(1), d(0), d(1) }) {}

textured_quad2::~textured_quad2()
{
	if (this->m_texture) delete this->m_texture;
}

void textured_quad2::set_texture(texture *texture)
{
	this->m_texture = texture;
}

void textured_quad2::update_uv()
{
	// order compatible with the generic quad2 implementation: 1 2 3 1 3 4
	this->m_data[2]  = this->m_texture->m_uv[0](0); // 1
	this->m_data[3]  = this->m_texture->m_uv[0](1); // 1
	this->m_data[6]  = this->m_texture->m_uv[1](0); // 2
	this->m_data[7]  = this->m_texture->m_uv[1](1); // 2
	this->m_data[10] = this->m_texture->m_uv[2](0); // 3
	this->m_data[11] = this->m_texture->m_uv[2](1); // 3
	this->m_data[14]  = this->m_texture->m_uv[0](0); // 1
	this->m_data[15]  = this->m_texture->m_uv[0](1); // 1
	this->m_data[18]  = this->m_texture->m_uv[2](0); // 3
	this->m_data[19]  = this->m_texture->m_uv[2](1); // 3
	this->m_data[22] = this->m_texture->m_uv[3](0); // 4
	this->m_data[23] = this->m_texture->m_uv[3](1); // 4
	
	// Also: don't update anything related to the positions of the vertices
}