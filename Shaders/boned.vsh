layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_weight;
layout (location = 2) in uvec4 i_index;
layout (location = 3) in vec3 i_nor;
layout (location = 4) in vec4 i_col;
layout (location = 5) in vec2 i_uv[4];

out vec2 f_uv[4];
out vec4 f_col;

uniform vec3 u_col;

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform mat4 u_bone[55];

void main()
{
	// Get skinned position and normal
	vec3 skin_pos = vec3(0.0f);
	vec3 skin_nor = vec3(0.0f);

	for (int i = 0; i < 3; i++)
	{
		mat4 bone = u_bone[i_index[i]];
		float weight = i_weight[i];
		skin_pos += vec3(bone * vec4(i_pos, 1.0f)) * weight;
		skin_nor += vec3(bone * vec4(i_nor, 0.0f)) * weight;
	}

	// Transform
	vec4 pos = (u_p * u_v * u_m) * vec4(skin_pos, 1.0f);
	vec3 nor = (u_m * vec4(skin_nor, 0.0f)).xyz;
	vec3 vnor = (u_v * vec4(nor, 0.0f)).xyz;

	gl_Position = pos;

	// Pass pass information
	for (uint i = 0u; i < u_passes; i++)
	{
		uint flag = u_pass_flag[i];

		// Check if environment mapped
		if ((flag & MATFLAG_ENVIRONMENT) != 0u)
		{
			// Pass reflection vector
			// TODO: This is not correct
			f_uv[i] = (u_env_mat[i] * vec4(reflect(normalize(pos.xyz), vnor), 1.0f)).xy;
		}
		else
		{
			// Pass texture coordinates
			f_uv[i] = i_uv[i];
		}
	}

	// Pass vertex color
	f_col = vec4((i_col.rgb * 2.0f) * u_col, i_col.a * 2.0f) * vec4(vec3(0.675f + dot(nor, vec3(0.707106f, 0.707106f, 0.0f)) * 0.325f), 1.0f);
}