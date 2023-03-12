#version 330 core

in vec2 tex_coord;

uniform sampler2D u_base_texture;
uniform sampler2D u_normal_map;
uniform vec3 u_light_dir;

void main()
{
	vec4 color = texture2D(u_base_texture, tex_coord);
	vec3 normal = texture2D(u_normal_map, tex_coord).xyz * 2 - 1;
	
	float lightness = dot(u_light_dir, normal);
	color.xyz *= 1 - ((1 - lightness) * (1 - lightness));
	
	gl_FragColor = color;
}