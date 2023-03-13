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
	local.x *= -1;
	
	vec2 alpha = vec2(sin(u_angle), cos(u_angle)); // this has nothing to do with the alpha value of the render texture
	
	
	float max2min = sign(single_component_cross(u_alpha_max, u_alpha_min));
	float local2min = sign(single_component_cross(u_alpha_min, local));
	float max2local = sign(single_component_cross(local, u_alpha_max));
	float a2local = sign(single_component_cross(alpha, local));
	float a2min = sign(single_component_cross(alpha, u_alpha_min));
	
	float v0 = min(1, max(0, local2min + (1-a2local) - a2min));
	float v1 = min(1, max(0, local2min + max2local - max2min));
	
	float circle = clamp(0.5*u_bounds.z-length(local), 0, 1);
	
	vec3 unsel = vec3(0, 0, 0);
	vec3 sel = vec3(1, 0, 1);
	vec3 col = v0*sel + (1-v0)*unsel;
	
	color = vec4(col, v1 * circle);
}
