#version 330 core
in vec2 f_uv;
in vec4 f_col;

layout (location = 0) out vec4 o_col;

uniform sampler2D u_texture[4];

void main()
{
	o_col = texture(u_texture[0], f_uv) * f_col;
}