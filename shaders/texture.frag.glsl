#version 330 core

in vec2 tex_coord;

uniform sampler2D u_base_texture;
uniform sampler2D u_normal_map;
uniform vec3 u_light_dir;
uniform mat4 u_rotation_matrix;

void main()
{
	vec4 color = texture2D(u_base_texture, tex_coord);
	vec3 normal = texture2D(u_normal_map, tex_coord).xyz * 2 - 1;
	vec4 light_dir = inverse(u_rotation_matrix) * vec4(u_light_dir, 1.0);
	
	float lightness = dot(light_dir.xyz, normal);
	color.xyz *= 1 - ((1 - lightness) * (1 - lightness));
	
	gl_FragColor = color;
}