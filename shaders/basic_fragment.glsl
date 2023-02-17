#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 color;

uniform float u_corner_radius;
uniform float u_edge_smoothness;
uniform float u_stroke_weight;
uniform vec4 u_stroke_color;
uniform vec4 u_bounds;

void main()
{
	vec2 local_position = vec2(gl_FragCoord.x - u_bounds.x, 480 - gl_FragCoord.y - u_bounds.y);
	float max_width = min(u_bounds.z, u_bounds.w) * 0.5;
	float corner_size = min(u_corner_radius, max_width);
	
	// compute signed distance
	float dx = 1.0 - min(local_position.x, u_bounds.z - local_position.x) / corner_size;
	float dy = 1.0 - min(local_position.y, u_bounds.w - local_position.y) / corner_size;
	float signed_distance;
	if (dx > 0 && dy > 0) // this is a corner pixel
		signed_distance = sqrt(dx*dx + dy*dy);
	else
		signed_distance = max(dx, dy);
	signed_distance = 1.0 - signed_distance;
	
	// compute alpha values for the corner sections
	float alpha_corner_radius = 1.0;
	if (u_corner_radius != 0) alpha_corner_radius = clamp(signed_distance * corner_size / u_edge_smoothness, 0, 1.0);
	
	// compute alpha values for edge stroke
	float alpha_edge_stroke = 0.0;
	if (u_stroke_weight != 0.0) alpha_edge_stroke = 1.0 - clamp(((signed_distance * corner_size) - u_stroke_weight) / u_edge_smoothness, 0, 1);
	
	vec4 out_color = vec4(local_position.x / u_bounds.z, local_position.y / u_bounds.w, 0, 1.0); // TODO: sample texture/find background color
	out_color.xyz = out_color.xyz * (1.0 - alpha_edge_stroke) + u_stroke_color.xyz * u_stroke_color.w * alpha_edge_stroke;
	
	color = vec4(out_color.xyz, alpha_corner_radius);
}
