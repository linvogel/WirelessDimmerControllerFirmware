#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 color;

uniform vec2 u_alpha_min;
uniform vec2 u_alpha_max;
uniform vec2 u_center;
uniform float u_angle;



void main()
{
	vec2 local = gl_FragCoord.xy - u_center;
	
	vec2 alpha = vec2(sin(u_angle), cos(u_angle));
}
