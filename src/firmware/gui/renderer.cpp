#include "renderer.hpp"
#include "../errors.hpp"

#include <fstream>
#include <cmath>

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

dim::gui::renderer::renderer(GLFWwindow *window)
{
	this->m_window = window;
	
	glfwMakeContextCurrent(this->m_window);
	
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
	
	info("Loading shaders...");
	this->m_base_program = this->createShader("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");
	this->m_current_program = this->m_base_program;
	GL_CALL(glUseProgram(this->m_current_program));
	
	this->reset();
	
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glEnable( GL_BLEND ));
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

dim::gui::renderer::~renderer()
{
	glDeleteProgram(this->m_base_program);
}

unsigned int dim::gui::renderer::create_2d_float_vertex_buffer_simple(size_t size, float* data)
{
	unsigned int buffer;
	GL_CALL(glGenBuffers(1, &buffer));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
	
	return buffer;
}

void dim::gui::renderer::update_buffer(unsigned int buffer, size_t size, float* data)
{
	if (buffer != 0) {
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW));
	}
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

int dim::gui::renderer::createShader(const std::string &vertex_shader_file, const std::string &fragment_shader_file)
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

void dim::gui::renderer::reset()
{
	this->m_proj_stack.clear();
	this->m_view_stack.clear();
	this->m_model_stack.clear();
	this->m_proj_stack.push_back(dim::math::matrix4f::identity());
	this->m_view_stack.push_back(dim::math::matrix4f::identity());
	this->m_model_stack.push_back(dim::math::matrix4f::identity());
	GL_CALL(glViewport(0, 0, 800, 480));
	this->ortho(0, 800, 480, 0, -1, 1);
	this->update_mvp();
}


void dim::gui::renderer::wait()
{
	glfwWaitEvents();
}

void dim::gui::renderer::wait(double timeout)
{
	glfwWaitEventsTimeout(timeout);
}

void dim::gui::renderer::poll()
{
	glfwPollEvents();
}

void dim::gui::renderer::swap()
{
	glfwSwapBuffers(this->m_window);
}

void dim::gui::renderer::set_swap_interval(int i)
{
	glfwSwapInterval(i);
}

void dim::gui::renderer::push_proj()
{
	this->m_proj_stack.push_back(this->m_proj_stack.back());
}

void dim::gui::renderer::pop_proj()
{
	if (this->m_proj_stack.size() > 1) this->m_proj_stack.pop_back();
}

void dim::gui::renderer::push_view()
{
	this->m_view_stack.push_back(this->m_view_stack.back());
}

void dim::gui::renderer::pop_view()
{
	if (this->m_view_stack.size() > 1) this->m_view_stack.pop_back();
}

void dim::gui::renderer::push_model()
{
	this->m_model_stack.push_back(this->m_model_stack.back());
}

void dim::gui::renderer::pop_model()
{
	if (this->m_model_stack.size() > 1) this->m_model_stack.pop_back();
}

void dim::gui::renderer::ortho(float left, float right, float bottom, float top, float near, float far)
{
	dim::math::matrix4f mat = {
		(float)(2.0 / (right - left)), 0.0, 0.0, -((right + left) / (right - left)),
		0.0, (float)(2.0 / (top - bottom)), 0.0, -((top + bottom) / (top - bottom)),
		0.0, 0.0, (float)(-2.0 / (far - near)), -((far + near) / (far - near)),
		0.0, 0.0, 0.0, 1.0
	};
	
	mat *= this->m_proj_stack.back();
	this->m_proj_stack.back() = mat;
}

void dim::gui::renderer::update_mvp()
{
	dim::math::matrix4f mvp = this->m_proj_stack.back() * this->m_view_stack.back() * this->m_model_stack.back();
	int location;
	GL_CALL(location = glGetUniformLocation(this->m_current_program, "MVP"));
	dim::math::matrix4f &mat = this->m_proj_stack.back();
	if (location != -1) GL_CALL(glUniformMatrix4fv(location, 1, GL_TRUE, mvp.get_data()));
}

void dim::gui::renderer::transform(dim::math::vector2f pos, float angle, dim::math::vector2f scale)
{
	this->translate(pos, false);
	this->rotate(angle, false);
	this->scale(scale, true);
}

void dim::gui::renderer::translate(dim::math::vector2f position, bool update_uniform_mvp)
{
	dim::math::matrix4f mat = dim::math::matrix4f::identity();
	mat(3, 0) = position(0);
	mat(3, 1) = position(1);
	mat *= this->m_proj_stack.back();
	this->m_proj_stack.back() = mat;
		
	if (update_uniform_mvp) this->update_mvp();
}

void dim::gui::renderer::rotate(float angle, bool update_uniform_mvp)
{
	dim::math::matrix4f mat = dim::math::matrix4f::identity();
	float s = static_cast<float>(std::sin(static_cast<double>(angle)));
	float c = static_cast<float>(std::cos(static_cast<double>(angle)));
	mat(0, 0) = c;
	mat(1, 1) = c;
	mat(1, 0) = -s;
	mat(0, 1) = s;
	mat *= this->m_proj_stack.back();
	this->m_proj_stack.back() = mat;
	
	if (update_uniform_mvp) this->update_mvp();
}

void dim::gui::renderer::scale(dim::math::vector2f scale, bool update_uniform_mvp)
{
	dim::math::matrix4f mat = dim::math::matrix4f::identity();
	mat(0, 0) = scale(0);
	mat(1, 1) = scale(1);
	mat *= this->m_proj_stack.back();
	this->m_proj_stack.back() = mat;
	
	if (update_uniform_mvp) this->update_mvp();
}

void dim::gui::renderer::clear()
{
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void dim::gui::renderer::draw_shape(dim::gui::shape2 *shape)
{
	if (shape->m_buffer == 0) {
		error("cannot render buffer, buffer not initialized on gpu!");
		return;
	}
	
	int location = glGetUniformLocation(this->m_current_program, "u_corner_radius");
	glUniform1f(location, shape->m_corner_radius);
	location = glGetUniformLocation(this->m_current_program, "u_bounds");
	glUniform4f(location, shape->m_bounds.x, shape->m_bounds.y, shape->m_bounds.w, shape->m_bounds.h);
	location = glGetUniformLocation(this->m_current_program, "u_edge_smoothness");
	glUniform1f(location, shape->m_edge_smoothness);
	location = glGetUniformLocation(this->m_current_program, "u_stroke_weight");
	glUniform1f(location, shape->m_stroke_weight);
	location = glGetUniformLocation(this->m_current_program, "u_stroke_color");
	glUniform4fv(location, 1, shape->m_stroke_color.get_data());
	location = glGetUniformLocation(this->m_current_program, "u_bg_color");
	glUniform4fv(location, 1, shape->m_background_color.get_data());
	
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, shape->m_buffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, shape->size() * sizeof(float), shape->data(), GL_STREAM_DRAW));
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	dim::gui::line2 *line;
	dim::gui::triang2 *tri;
	dim::gui::quad2 *quad;
	dim::gui::circle2 *circle;
	
	if (line = dynamic_cast<dim::gui::line2*>(shape)) {
		// draw line
		GL_CALL(glDrawArrays(GL_LINES, 0, 2));
	} else if (tri = dynamic_cast<dim::gui::triang2*>(shape)) {
		// draw triangle
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
	} else if (quad = dynamic_cast<dim::gui::quad2*>(shape)) {
		// draw quad
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
	} else if (circle = dynamic_cast<dim::gui::circle2*>(shape)) {
		// draw circle
		GL_CALL(glDrawArrays(GL_TRIANGLE_FAN, 0, N_VERT_CIRCLE + 2));
	} else {
		warn("Invalid shape type!");
	}
	
	GL_CALL(glDisableVertexAttribArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}