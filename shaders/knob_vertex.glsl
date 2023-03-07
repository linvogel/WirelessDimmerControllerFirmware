#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec2 position;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position.xy, 0.0, 1.0);
}
