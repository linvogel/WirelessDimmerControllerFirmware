#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec2 position;

uniform mat4 MVP;
uniform vec2 u_center;
uniform float u_scale;


void main()
{
	vec2 small_diff = position - u_center;
	vec4 newpos = MVP * vec4(position, 0.0f, 1.0f);
	vec4 newcenter = MVP * vec4(u_center, 0.0f, 1.0f);
	vec2 diff = newpos.xy - newcenter.xy;
	float len = length(small_diff);
	float scale = (len + u_scale) / len;
	gl_Position = newcenter + vec4(diff*scale, 0.0f, 0.0f);
	//gl_Position = newpos;
}
