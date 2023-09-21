layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec2 i_uv;

out vec2 f_uv;

void main()
{
	gl_Position = i_pos;
	f_uv = i_uv;
}