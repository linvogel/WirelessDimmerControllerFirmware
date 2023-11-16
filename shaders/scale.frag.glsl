#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 color;

uniform float u_corner_radius;
uniform float u_edge_smoothness;
uniform float u_stroke_weight;
uniform vec4 u_stroke_color;
uniform vec4 u_bounds;
uniform float u_scale;

void main()
{
	vec2 local_position = vec2(gl_FragCoord.x - u_bounds.x + u_scale, u_bounds.w - gl_FragCoord.y - u_bounds.y - u_scale);
	float max_width = min(u_bounds.z, u_bounds.w) * 0.5 + u_scale;
	float corner_size = min(u_corner_radius, max_width) + u_scale;
	
	// compute signed distance
	float dx = 1.0 - min(local_position.x, u_bounds.z + 2*u_scale - local_position.x) / corner_size;
	float dy = 1.0 - min(local_position.y + 2*u_scale, u_bounds.w - local_position.y) / corner_size;
	float signed_distance;
	if (dx > 0 && dy > 0) // this is a corner pixel
		signed_distance = 1 - sqrt(dx*dx + dy*dy);
	else
		signed_distance = 1 - max(dx, dy);
	
	float alpha = 1 - clamp(abs((signed_distance - (u_scale / max_width / 2)) * corner_size) / u_stroke_weight, 0, 1);
	
	color = vec4(u_stroke_color.xyz, alpha);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
