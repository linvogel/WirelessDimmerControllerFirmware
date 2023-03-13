#include "renderer.hpp"
#include "../errors.hpp"

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif
#define MODULE_NAME "renderer"
#include "../logging.hpp"

#include <fstream>
#include <cmath>

using namespace dim::gui;

#ifndef NDEBUG
#define GL_CALL(x) \
{\
	x;\
	unsigned int result = glGetError();\
	if (result != GLEW_OK) {\
		error("OpenGL ERROR: %s", glewGetErrorString(result));\
	}\
}
#else
#define GL_CALL(x) x
#endif

renderer::renderer(GLFWwindow *window)
{
	this->m_font_atlas = 0;
	this->m_window = window;
	glfwMakeContextCurrent(this->m_window);
	
	// TODO: this should be configurable
	this->m_font_name = "fonts/arial.ttf";
	
	this->text_color = {1.0f, 1.0f, 1.0f, 1.0f};
	
	init_text_rendering();
	
	debug("Initializing GLEW...");
	if (glewInit() != GLEW_OK) {
		fatal("GLEW could not be initialized.");
		exit(dim::GLEW_INIT_FAIL);
	}
	info("GLEW initialized successfully!");
	info("Running with OpenGL version '%s'", glGetString(GL_VERSION));
	
	this->m_proj_stack.reserve(128);
	this->m_view_stack.reserve(128);
	this->m_model_stack.reserve(128);
	this->m_rotation_stack.reserve(128);
	
	info("Loading shaders...");
	this->m_base_program = this->createShader("shaders/basic.vert.glsl", "shaders/basic.frag.glsl");
	this->m_text_program = this->createShader("shaders/font.vert.glsl", "shaders/font.frag.glsl");
	this->m_texture_program = this->createShader("shaders/texture.vert.glsl", "shaders/texture.frag.glsl");
	this->m_knob_program = this->createShader("shaders/knob.vert.glsl", "shaders/knob.frag.glsl");
	this->m_current_program = this->m_base_program;
	GL_CALL(glUseProgram(this->m_current_program));
	
	this->reset();
	
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glEnable( GL_BLEND ));
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	
	GL_CALL(glGenBuffers(1, &(this->m_text_vertex_buffer)));
}

void renderer::init_text_rendering()
{
	debug("Initializing text rendering...");
	
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		error("Unable to load freetype, text will not be rendererd!");
		return;
	}
	
	verbose("Creating font face...");
	FT_Face face;
	if (FT_New_Face(ft, this->m_font_name.c_str(), 0, &face)) {
		error("Unable to load font '%s'! Fonts will not be rendered!", this->m_font_name.c_str());
		return;
	}
	
	verbose("Set general font parameters...");
	const int font_size = 96;
	FT_Set_Pixel_Sizes(face, 0, font_size); // large enough so as to never encounter it, this may change
	
	// clear font texture and atlas
	if (this->m_font_atlas) GL_CALL(glDeleteTextures(1, &(this->m_font_atlas)));
	this->m_font_map.clear();
	
	this->bitmap_width = (font_size+2)*20;
	this->bitmap_height = (font_size+2)*20;
	auto idx = [&](int x, int y) { return x + this->bitmap_width * y; };
	std::vector<uint8_t> atlas_data(this->bitmap_width * this->bitmap_height, 0); // generate space for approx 400 characters
	
	int offset_x = 1;
	int offset_y = 1;
	
	verbose("Creating character atlas...");
	for (int i = 0; i < 256; i++) { // NOTE: this generates all ascii and extended ascii codes, maybe there is a better way to do that
		char c = (char)i; // nothing larger that 255 is possible here, see loop condition
		
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			error("Character '%c' cannot be loaded. This character will not be rendererd");
			// TODO: replace glphy in map with something
			continue;
		}
		
		glyph g = {};
		g.size = { (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows };
		g.bearing = { (int)face->glyph->bitmap_left, (int)face->glyph->bitmap_top };
		g.advance = face->glyph->advance.x;
		
		if (offset_x + (int)face->glyph->bitmap.width + 2 >= bitmap_width) {
			offset_y += font_size + 2;
			offset_x = 0;
		}
		g.origin = {offset_x+1, offset_y+1};
		offset_x += face->glyph->bitmap.width + 2;
		
		for (int y = 0; y < g.size(1); y++) {
			for (int x = 0; x < g.size(0); x++) {
				atlas_data[idx(x+g.origin(0), y+g.origin(1))] = face->glyph->bitmap.buffer[x + y*face->glyph->bitmap.width];
			}
		}
		
		this->m_font_map.insert(std::make_pair(c, g));
		
	}
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glGenTextures(1, &(this->m_font_atlas));
	glBindTexture(GL_TEXTURE_2D, this->m_font_atlas);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap_width, bitmap_height, 0, GL_RED, GL_UNSIGNED_BYTE, atlas_data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	
	info("Text rendering initialized successfully!");
}

renderer::~renderer()
{

}

unsigned int renderer::create_2d_float_vertex_buffer_simple(size_t size, custom_array<float> &data)
{
	unsigned int buffer;
	GL_CALL(glGenBuffers(1, &buffer));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data.get(), GL_STATIC_DRAW));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
	
	return buffer;
}

void renderer::update_buffer(unsigned int buffer, size_t size, custom_array<float> &data)
{
	trace("renderer::update_buffer");
	if (buffer != 0) {
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data.get(), GL_STATIC_DRAW));
	}
	trace("renderer::update_buffer done.");
}

static int compileShader(unsigned int type, const std::string &file_name)
{
	unsigned int id;
	GL_CALL(id = glCreateShader(type));
	std::string source = "";
	char line[1024];
	std::ifstream istr(file_name, std::ios_base::in);
	if (istr.is_open()) {
		while (istr.good()) {
			istr.getline(line, 1024);
			source += line;
			source += "\n";
		}
	} else {
		error("Cannot open file: %s", file_name.c_str());
		return 0;
	}
	
	const char* src = source.c_str();
	GL_CALL(glShaderSource(id, 1, &src, nullptr));
	GL_CALL(glCompileShader(id));
	
	int result;
	GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* msg = (char*)alloca(length * sizeof(char));
		GL_CALL(glGetShaderInfoLog(id, length, &length, msg));
		printf("Error while compiling shader: %s\n", file_name.c_str());
		printf("%s\n", msg);
	}
	
	return id;
}

int renderer::createShader(const std::string &vertex_shader_file, const std::string &fragment_shader_file)
{
	unsigned int program;
	GL_CALL(program = glCreateProgram());
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader_file);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader_file);
	
	GL_CALL(glAttachShader(program, vs));
	GL_CALL(glAttachShader(program, fs));
	GL_CALL(glLinkProgram(program));
	GL_CALL(glValidateProgram(program));
	
	GL_CALL(glDeleteShader(vs));
	GL_CALL(glDeleteShader(fs));
	
	return program;
}

void renderer::reset()
{
	this->m_proj_stack.clear();
	this->m_view_stack.clear();
	this->m_model_stack.clear();
	this->m_rotation_stack.clear();
	this->m_proj_stack.push_back(matrix4f::identity());
	this->m_view_stack.push_back(matrix4f::identity());
	this->m_model_stack.push_back(matrix4f::identity());
	this->m_rotation_stack.push_back(matrix4f::identity());
	GL_CALL(glViewport(0, 0, 800, 480));
	this->ortho(0, 800, 480, 0, -1, 1);
	this->update_mvp();
}


void renderer::wait()
{
	glfwWaitEvents();
}

void renderer::wait(double timeout)
{
	glfwWaitEventsTimeout(timeout);
}

void renderer::poll()
{
	glfwPollEvents();
}

void renderer::swap()
{
	glfwSwapBuffers(this->m_window);
}

void renderer::set_swap_interval(int i)
{
	glfwSwapInterval(i);
}

void renderer::push_proj()
{
	this->m_proj_stack.push_back(this->m_proj_stack.back());
	push_rotation();
}

void renderer::pop_proj()
{
	if (this->m_proj_stack.size() > 1) this->m_proj_stack.pop_back();
	pop_rotation();
}

void renderer::push_view()
{
	this->m_view_stack.push_back(this->m_view_stack.back());
}

void renderer::pop_view()
{
	if (this->m_view_stack.size() > 1) this->m_view_stack.pop_back();
}

void renderer::push_model()
{
	this->m_model_stack.push_back(this->m_model_stack.back());
}

void renderer::pop_model()
{
	if (this->m_model_stack.size() > 1) this->m_model_stack.pop_back();
}

void renderer::push_rotation()
{
	this->m_rotation_stack.push_back(this->m_rotation_stack.back());
}

void renderer::pop_rotation()
{
	if (this->m_rotation_stack.size() > 1) this->m_rotation_stack.pop_back();
}

void renderer::ortho(float left, float right, float bottom, float top, float near, float far)
{
	matrix4f mat = {
		(float)(2.0 / (right - left)), 0.0,                               0.0,                          -((right + left) / (right - left)),
		0.0,                           (float)(2.0 / (top - bottom)),     0.0,                          -((top + bottom) / (top - bottom)),
		0.0,                           0.0,                               (float)(-2.0 / (far - near)), -((far + near) / (far - near)),
		0.0,                           0.0,                               0.0,                          1.0
	};
	
	//mat *= this->m_proj_stack.back();
	this->m_proj_stack.back() = mat;
}

void renderer::update_mvp()
{
	this->mvp = this->m_proj_stack.back() * this->m_view_stack.back() * this->m_model_stack.back();
	int location;
	GL_CALL(location = glGetUniformLocation(this->m_current_program, "MVP"));
	if (location != -1) GL_CALL(glUniformMatrix4fv(location, 1, GL_TRUE, this->mvp.get_data()));
}

void renderer::reload_mvp()
{
	int location;
	GL_CALL(location = glGetUniformLocation(this->m_current_program, "MVP"));
	if (location != -1) GL_CALL(glUniformMatrix4fv(location, 1, GL_TRUE, this->mvp.get_data()));
}

void renderer::transform(vector2f pos, float angle, vector2f scale)
{
	this->scale(scale, false);
	this->rotate(angle, false);
	this->translate(pos, true);
}

void renderer::translate(vector2f position, bool update_uniform_mvp)
{
	matrix4f mat = matrix4f::identity();
	vector4f pos = { position(0), position(1), 0, 0 };
	pos = this->m_proj_stack.back() * pos;
	mat(3, 0) = pos(0);
	mat(3, 1) = pos(1);
	mat *= this->m_proj_stack.back();
	this->m_proj_stack.back() = mat;
		
	if (update_uniform_mvp) this->update_mvp();
}

void renderer::rotate(float angle, bool update_uniform_mvp)
{
	matrix4f mat = matrix4f::identity();
	float s = static_cast<float>(std::sin(static_cast<double>(angle)));
	float c = static_cast<float>(std::cos(static_cast<double>(angle)));
	mat(0, 0) = c;
	mat(1, 1) = c;
	mat(1, 0) = s;
	mat(0, 1) = -s;
	matrix4f &m = this->m_rotation_stack.back();
	matrix4f mat1 = this->m_rotation_stack.back() * mat;
	mat = this->m_proj_stack.back() * mat;
	this->m_rotation_stack.back() = mat1;
	this->m_proj_stack.back() = mat;
	
	if (update_uniform_mvp) this->update_mvp();
}

void renderer::scale(vector2f scale, bool update_uniform_mvp)
{
	matrix4f mat = matrix4f::identity();
	mat(0, 0) = scale(0);
	mat(1, 1) = scale(1);
	mat = mat * this->m_proj_stack.back();
	this->m_proj_stack.back() = mat;
	
	if (update_uniform_mvp) this->update_mvp();
}

void renderer::clear()
{
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void renderer::draw_shape(shape2 *shape)
{
	if (shape->m_buffer == 0) {
		error("cannot render buffer, buffer not initialized on gpu!");
		return;
	}
	
	int location;
	if (shape->is_textured()) {
		// for now, only textured quads are supported
		textured_quad2 *tquad = dynamic_cast<textured_quad2*>(shape);
		if (tquad == nullptr) {
			error("Textured shape is not textured_quad2!!! Only textured_quad2 is supported for now");
			return;
		}
		
		this->m_current_program = this->m_texture_program;
		GL_CALL(glUseProgram(this->m_current_program));
		
		this->reload_mvp();
		
		GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_base_texture"));
		GL_CALL(glUniform1i(location, 1)); // GL_TEXTURE1
		GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_normal_map"));
		GL_CALL(glUniform1i(location, 2)); // GL_TEXTURE2
		GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_light_dir"));
		GL_CALL(glUniform3fv(location, 1, vector3f({1.0f, 1.0f, 2.0f}).normalize().get_data()));
		GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_rotation_matrix")); // pass rotation matrix
		GL_CALL(glUniformMatrix4fv(location, 1, GL_TRUE, this->m_rotation_stack.back().get_data()));
		
		
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, shape->m_buffer));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, shape->size() * sizeof(float), shape->data().get(), GL_STREAM_DRAW));
		GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0));
		GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)8));
		GL_CALL(glEnableVertexAttribArray(0));
		GL_CALL(glEnableVertexAttribArray(1));
		
		// bind texture
		GL_CALL(glActiveTexture(GL_TEXTURE1));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, tquad->m_texture->m_base_texture));
		GL_CALL(glActiveTexture(GL_TEXTURE2));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, tquad->m_texture->m_normal_texture));
		
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
		
		GL_CALL(glDisableVertexAttribArray(0));
		GL_CALL(glDisableVertexAttribArray(1));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	} else {
		this->m_current_program = shape->m_special_program;
		GL_CALL(glUseProgram(this->m_current_program));
		
		this->reload_mvp();
		
		// if this the base program is used, populate the necessary variables
		// otherwise, the caller must have already set the variables by itself
		if (this->m_current_program == this->m_base_program) { 
			GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_corner_radius"));
			GL_CALL(glUniform1f(location, shape->m_corner_radius));
			GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_bounds"));
			GL_CALL(glUniform4f(location, shape->m_bounds.x + shape->m_offset(0), shape->m_bounds.y + shape->m_offset(1), shape->m_bounds.w, shape->m_bounds.h));
			GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_edge_smoothness"));
			GL_CALL(glUniform1f(location, shape->m_edge_smoothness));
			GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_stroke_weight"));
			GL_CALL(glUniform1f(location, shape->m_stroke_weight));
			GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_stroke_color"));
			GL_CALL(glUniform4fv(location, 1, shape->m_stroke_color.get_data()));
			GL_CALL(location = glGetUniformLocation(this->m_current_program, "u_bg_color"));
			GL_CALL(glUniform4fv(location, 1, shape->m_background_color.get_data()));
		}
		
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, shape->m_buffer));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, shape->size() * sizeof(float), shape->data().get(), GL_STREAM_DRAW));
		GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));
		GL_CALL(glEnableVertexAttribArray(0));
		line2 *line;
		triang2 *tri;
		quad2 *quad;
		circle2 *circle;
		
		if (line = dynamic_cast<line2*>(shape)) {
			// draw line
			GL_CALL(glDrawArrays(GL_LINES, 0, 2));
		} else if (tri = dynamic_cast<triang2*>(shape)) {
			// draw triangle
			GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
		} else if (quad = dynamic_cast<quad2*>(shape)) {
			// draw quad
			GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
		} else if (circle = dynamic_cast<circle2*>(shape)) {
			// draw circle
			GL_CALL(glDrawArrays(GL_TRIANGLE_FAN, 0, N_VERT_CIRCLE + 2));
		} else {
			warn("Invalid shape type!");
		}
		
		GL_CALL(glDisableVertexAttribArray(0));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}

vector2f renderer::get_text_size(std::string text, float size)
{
	float minx = size;
	float maxx = 0;
	float miny = size;
	float maxy = -size;
	float offset = 0;
	const float scale = size / 96.0f;
	
	glyph &first = this->m_font_map[text[0]];
	minx = (float)(first.bearing(0));
	
	for (char c : text) {
		glyph &g = this->m_font_map[c];
		maxx = offset + g.bearing(0) + g.size(0);
		offset += g.advance >> 6;
		maxy = std::max(maxy, (float)(g.bearing(1)));
		miny = std::min(maxy, (float)(g.bearing(1) - g.size(1)));
	}
	
	return { (maxx - minx) * scale, (maxy - miny) * scale };
}

void renderer::draw_text_centered(std::string text, float x, float y, float size)
{
	const float size_factor = size / 96.0f;
	this->m_current_program = this->m_text_program;
	glUseProgram(this->m_current_program);
	
	this->reload_mvp();
	
	int location;
	location = glGetUniformLocation(this->m_current_program, "u_text_color");
	glUniform4fv(location, 1, text_color.get_data());
	
	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_text_vertex_buffer);
	glBindTexture(GL_TEXTURE_2D, this->m_font_atlas);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
	
	vector2f rendered_size = this->get_text_size(text, size);
	
	float offset_x = x - rendered_size(0) * 0.5f;
	float offset_y = y + rendered_size(1) * 0.5f;
	
	for (char c : text) {
		const glyph &g = this->m_font_map[c];
		float uv_x = (float)(g.origin(0)) / (float)(this->bitmap_width);
		float uv_y = (float)(g.origin(1)) / (float)(this->bitmap_height);
		float uv_w = (float)(g.size(0)) / (float)(this->bitmap_width);
		float uv_h = (float)(g.size(1)) / (float)(this->bitmap_height);
		
		float local_x = std::ceil(offset_x + (size_factor * (float)(g.bearing(0))));
		float local_y = std::ceil(offset_y - (size_factor * (float)(g.bearing(1))));
		float local_w = std::ceil(size_factor * (float)(g.size(0)));
		float local_h = std::ceil(size_factor * (float)(g.size(1)));
		
		float data[6*4] = {
			local_x        , local_y        , uv_x     , uv_y     ,
			local_x+local_w, local_y        , uv_x+uv_w, uv_y     ,
			local_x        , local_y+local_h, uv_x     , uv_y+uv_h,
			local_x+local_w, local_y        , uv_x+uv_w, uv_y     ,
			local_x+local_w, local_y+local_h, uv_x+uv_w, uv_y+uv_h,
			local_x        , local_y+local_h, uv_x     , uv_y+uv_h,
		};
		
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		offset_x += std::ceil(size_factor * (float)(g.advance >> 6)); // the advance is given in a unit of 1/64th pixel, so divide by 64 to get in pixels
	}
	
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int renderer::create_texture(uint8_t *data, int width, int height, int bpp, int channels)
{
	verbose("Creating texture...");
	unsigned int texture;
	GL_CALL(glGenTextures(1, &texture));
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	verbose("Texture created.");
	
	return texture;
}

void renderer::set_program(unsigned int program)
{
	this->m_current_program = program;
	GL_CALL(glUseProgram(this->m_current_program));
}

void renderer::set_uniform_scalar(unsigned int program, const char* name, float value)
{
	int location;
	GL_CALL(location = glGetUniformLocation(program, name));
	GL_CALL(glUniform1f(location, value));
}

void renderer::set_uniform_vec2(unsigned int program, const char* name, float *value)
{
	int location;
	GL_CALL(location = glGetUniformLocation(program, name));
	verbose("Pointer: %p, location: %d, name: %s", value, location, name);
	GL_CALL(glUniform2fv(location, 1, value));
}

void renderer::set_uniform_vec3(unsigned int program, const char* name, float *value)
{
	int location;
	GL_CALL(location = glGetUniformLocation(program, name));
	GL_CALL(glUniform3fv(location, 1, value));
}

void renderer::set_uniform_vec4(unsigned int program, const char* name, float *value)
{
	int location;
	GL_CALL(location = glGetUniformLocation(program, name));
	GL_CALL(glUniform4fv(location, 1, value));
}

void renderer::set_uniform_mat2(unsigned int program, const char* name, float *value)
{
	int location;
	GL_CALL(location = glGetUniformLocation(program, name));
	GL_CALL(glUniformMatrix2fv(location, 1, GL_FALSE, value));
}

void renderer::set_uniform_mat3(unsigned int program, const char* name, float *value)
{
	int location;
	GL_CALL(location = glGetUniformLocation(program, name));
	GL_CALL(glUniformMatrix3fv(location, 1, GL_FALSE, value));
}

void renderer::set_uniform_mat4(unsigned int program, const char* name, float *value)
{
	int location;
	GL_CALL(location = glGetUniformLocation(program, name));
	GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, value));
}
