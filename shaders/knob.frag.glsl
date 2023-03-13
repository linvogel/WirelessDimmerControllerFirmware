#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 color;

uniform vec2 u_alpha_min;
uniform vec2 u_alpha_max;
uniform vec2 u_center;
uniform float u_angle;


float single_component_cross(vec2 a, vec2 b)
{
	return a.x*b.y - a.y*b.x;
}

void main()
{
	vec2 local = gl_FragCoord.xy - u_center;
	
	vec2 alpha = vec2(sin(u_angle), cos(u_angle)); // this has nothing to do with the alpha value of the render texture
	
	float min_to_max = sign(single_component_cross(u_alpha_min, u_alpha_max));
	float min_to_alpha = sign(single_component_cross(u_alpha_min, alpha));
	float max_to_min = sign(single_component_cross(u_alpha_max, u_alpha_min));
	float max_to_alpha = sign(single_component_cross(u_alpha_min, alpha));
	
	float r = clamp(min_to_max * min_to_alpha,  0, 1);
	
	color = vec4(r, 0, 0, 1);
}
