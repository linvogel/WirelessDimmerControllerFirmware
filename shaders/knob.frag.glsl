#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 color;

uniform float u_angle;
uniform vec2 u_center_vec;
uniform vec2 u_alpha_min;
uniform vec2 u_alpha_max;
uniform vec4 u_bounds;


float single_component_cross(vec2 a, vec2 b)
{
	return a.x*b.y - a.y*b.x;
}

void main()
{
	vec2 local = vec2(gl_FragCoord.x - u_bounds.x, u_bounds.w - gl_FragCoord.y - u_bounds.y) - u_center_vec;
	
	vec2 alpha = vec2(sin(u_angle), cos(u_angle)); // this has nothing to do with the alpha value of the render texture
	
	float min_to_max = sign(single_component_cross(u_alpha_min, u_alpha_max));
	float min_to_alpha = sign(single_component_cross(u_alpha_min, alpha));
	float max_to_min = sign(single_component_cross(u_alpha_max, u_alpha_min));
	float max_to_alpha = sign(single_component_cross(u_alpha_min, alpha));
	float min_to_local = sign(single_component_cross(u_alpha_min, local));
	float local_to_alpha = sign(single_component_cross(local, alpha));
	
	float r = clamp(min_to_max * min_to_alpha,  0, 1);
	float g = clamp(max_to_min * max_to_alpha, 0, 1);
	float b = clamp(min_to_local * local_to_alpha, 0, 1);
	
	vec2 c = local * 0.01;
	color = vec4(abs(c.x), abs(c.y), b, 1);
}
