layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_uv;
layout (location = 2) in vec4 i_col;

out vec2 f_uv;
out vec4 f_col;

void main()
{
	gl_Position = vec4(-1.0 + i_pos.x / 320.0, 1.0 - i_pos.y / 240.0, i_pos.z, 1.0);
	f_uv = i_uv;
	f_col = i_col;
}