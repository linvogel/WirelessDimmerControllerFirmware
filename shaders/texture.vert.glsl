#version 330 core


// layout information
layout (location = 0) in vec2 world_position;
layout (location = 1) in vec2 uv_position;

out vec2 tex_coord;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(world_position.xy, 0.0, 1.0);
	tex_coord = uv_position;
}