#include "renderer.hpp"

dim::gui::renderer::renderer(GLFWwindow *window)
{
	this->m_window = window;
}

dim::gui::renderer::~renderer()
{
	
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

void dim::gui::renderer::push()
{
	glPushMatrix();
}

void dim::gui::renderer::pop()
{
	glPopMatrix();
}

void dim::gui::renderer::transform(dim::math::vector2f pos, float angle, dim::math::vector2f scale)
{
	glTranslated(pos(0), 0, pos(1));
	glRotatef(angle, 0, 1, 0);
	glScalef(scale(0), 1, scale(1));
}

void dim::gui::renderer::draw_shape(dim::gui::shape2 *shape)
{
	if (shape->m_buffer == 0) {
		error("cannot render buffer, buffer not initialized on gpu!");
		return;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, shape->m_buffer);
	dim::gui::line2 *line;
	dim::gui::triang2 *tri;
	dim::gui::quad2 *quad;
	dim::gui::circle2 *circle;
	
	if (line = dynamic_cast<dim::gui::line2*>(shape)) {
		// draw line
		glDrawArrays(GL_LINES, 0, 2);
	} else if (tri = dynamic_cast<dim::gui::triang2*>(shape)) {
		// draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);
	} else if (quad = dynamic_cast<dim::gui::quad2*>(shape)) {
		// draw quad
		glDrawArrays(GL_QUADS, 0, 4);
	} else if (circle = dynamic_cast<dim::gui::circle2*>(shape)) {
		// draw circle
		glDrawArrays(GL_TRIANGLE_FAN, 0, N_VERT_CIRCLE + 1);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}