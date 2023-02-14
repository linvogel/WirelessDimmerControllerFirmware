#include "shape2.hpp"

#include <GL/glew.h>
#include <memory>

#include "../logging.hpp"


dim::gui::shape2::shape2()
{
	this->m_nElements = 0;
	this->m_data = nullptr;
	this->m_buffer = 0;
}

dim::gui::shape2::shape2(size_t size, float *data)
{
	debug("creating shape2...");
	this->m_buffer = 0;
	this->m_nElements = size;
	this->m_data = (float*)std::malloc(this->m_nElements * sizeof(float));
	
	if (this->m_data == nullptr) {
		error("shape2 buffer allocation failed");
		return;
	}
	verbose("shape2 buffer allocated, copying supplied data...");
	
	if (data) std::memcpy(this->m_data, data, this->m_nElements * sizeof(float));
	
	verbose("creating gpu side shape2 buffer...");
	glGenBuffers(1, &(this->m_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer);
	glBufferData(GL_ARRAY_BUFFER, this->m_nElements * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind buffer
	verbose("shape2 created");
}

dim::gui::shape2::shape2(size_t size, std::initializer_list<float> values) : shape2(size)
{
	if (size != values.size()) {
		warn("shape2 init received initializer list that is not the same size as the declared buffer!");
	}
	if (this->m_data) {
		size_t i = 0;
		for (float f : values) {
			if (i == std::min(values.size(), size)) break;
			this->m_data[i++] = f;
		}
	}
}

dim::gui::shape2::~shape2()
{
	debug("destroying shape2");
	if (this->m_data) std::free(this->m_data);
}

void dim::gui::shape2::update_buffer(size_t size, float* data)
{
	verbose("updating shape2 buffer...");
	if (this->m_nElements != size) {
		verbose("resizing shpae2 buffer...");
		std::realloc(this->m_data, size * sizeof(float));
		this->m_nElements = size;
		// TODO: handle the opengl side of things
	}
	
	std::memcpy(this->m_data, data, this->m_nElements * sizeof(float));
	trace("shape2 buffer update done");
}


// line 2
dim::gui::line2::line2(float x1, float y1, float x2, float y2)
	: shape2(4, {x1, y1, x2, y2}) {}
dim::gui::line2::line2(dim::math::vector2f a, dim::math::vector2f b)
	: shape2(4, {a(0), a(1), b(0), b(1)}) {}

// triang 2
dim::gui::triang2::triang2(float x1, float y1, float x2, float y2, float x3, float y3)
	: shape2(6, {x1, y1, x2, y2, x3, y3}) {}
dim::gui::triang2::triang2(dim::math::vector2f a, dim::math::vector2f b, dim::math::vector2f c)
	: shape2(6, { a(0), a(1), b(0), b(1), c(0), c(1) }) {}

// quad 2
dim::gui::quad2::quad2(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
	: shape2(8, {x1, y1, x2, y2, x3, y3, x4, y4}) {}
dim::gui::quad2::quad2(dim::math::vector2f a, dim::math::vector2f b, dim::math::vector2f c, dim::math::vector2f d)
	: shape2(8, { a(0), a(1), b(0), b(1), c(0), c(1), d(0), d(1) }) {}

// circle 2
dim::gui::circle2::circle2(float x, float y, float r) : shape2(2*N_VERT_CIRCLE + 4)
{
	// prepare some space for the values
	dim::math::vector2f current({1, 0});
	dim::math::vector2f tmp;
	// prepare for geometric calculations
	double angle = PI2 / N_VERT_CIRCLE;
	float s = static_cast<float>(std::sin(angle));
	float c = static_cast<float>(std::cos(angle));
	dim::math::matrix2f rot_mat({c, -s, s, c});
	
	this->m_data[0] = x;
	this->m_data[1] = y;
	
	// rotate current and push scaled copies of it into storage
	for (int i = 1; i <= N_VERT_CIRCLE; i++) {
		tmp = current * r;
		current = rot_mat * current;
		this->m_data[2*i + 0] = tmp(0);
		this->m_data[2*i + 1] = tmp(1);
	}
	
	// duplicate the last point to close the circle
	this->m_data[2*N_VERT_CIRCLE + 2] = this->m_data[2];
	this->m_data[2*N_VERT_CIRCLE + 3] = this->m_data[3];
	
}