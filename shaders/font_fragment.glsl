#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_font_atlas;
uniform vec4 u_text_color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_font_atlas, TexCoords).r);
    color = u_text_color * sampled;
}  