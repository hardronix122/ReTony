in vec2 f_uv[4];
in vec4 f_col;

layout (location = 0) out vec4 o_col;

uniform sampler2D u_texture[4];

void main()
{
	vec4 accum = vec4(0.0f);
	for (uint i = 0u; i < u_passes; i++)
	{
		uint flag = u_pass_flag[i];

		vec4 texel = f_col;
		if ((flag & MATFLAG_PASS_IGNORE_VERTEX_ALPHA) != 0u)
			texel.a = 1.0f;

		if ((flag & MATFLAG_TEXTURED) != 0u)
		{
			if (i == 0u)
				texel *= texture(u_texture[0], f_uv[i]);
			else if (i == 1u)
				texel *= texture(u_texture[1], f_uv[i]);
			else if (i == 2u)
				texel *= texture(u_texture[2], f_uv[i]);
			else if (i == 3u)
				texel *= texture(u_texture[3], f_uv[i]);
		}

		accum += texel;
	}
	o_col = accum;
}