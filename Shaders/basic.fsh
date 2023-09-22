#version 330 core
const uint MATFLAG_UV_WIBBLE = (1u<<0u);
const uint MATFLAG_VC_WIBBLE = (1u<<1u);
const uint MATFLAG_TEXTURED = (1u<<2u);
const uint MATFLAG_ENVIRONMENT = (1u<<3u);
const uint MATFLAG_DECAL = (1u<<4u);
const uint MATFLAG_SMOOTH = (1u<<5u);
const uint MATFLAG_TRANSPARENT = (1u<<6u);
const uint MATFLAG_PASS_COLOR_LOCKED = (1u<<7u);
const uint MATFLAG_SPECULAR = (1u<<8u); // Specular lighting is enabled on this material (Pass0).
const uint MATFLAG_BUMP_SIGNED_TEXTURE = (1u<<9u); // This pass uses an offset texture which needs to be treated as signed data.
const uint MATFLAG_BUMP_LOAD_MATRIX = (1u<<10u); // This pass requires the bump mapping matrix elements to be set up.
const uint MATFLAG_PASS_TEXTURE_ANIMATES = (1u<<11u); // This pass has a texture which animates.
const uint MATFLAG_PASS_IGNORE_VERTEX_ALPHA = (1u<<12u); // This pass should not have the texel alpha modulated by the vertex alpha.
const uint MATFLAG_EXPLICIT_UV_WIBBLE = (1u<<14u); // Uses explicit uv wibble (set via script) rather than calculated.
const uint MATFLAG_WATER_EFFECT = (1u<<27u); // This material should be processed to provide the water effect.
const uint MATFLAG_NO_MAT_COL_MOD = (1u<<28u); // No material color modulation required (all passes have m.rgb = 0.5).

const uint vBLEND_MODE_DIFFUSE = 0u;
const uint vBLEND_MODE_ADD = 1u;
const uint vBLEND_MODE_ADD_FIXED = 2u;
const uint vBLEND_MODE_SUBTRACT = 3u;
const uint vBLEND_MODE_SUB_FIXED = 4u;
const uint vBLEND_MODE_BLEND = 5u;
const uint vBLEND_MODE_BLEND_FIXED = 6u;
const uint vBLEND_MODE_MODULATE = 7u;
const uint vBLEND_MODE_MODULATE_FIXED = 8u;
const uint vBLEND_MODE_BRIGHTEN = 9u;
const uint vBLEND_MODE_BRIGHTEN_FIXED = 10u;
const uint vBLEND_MODE_GLOSS_MAP = 11u;
const uint vBLEND_MODE_BLEND_PREVIOUS_MASK = 12u;
const uint vBLEND_MODE_BLEND_INVERSE_PREVIOUS_MASK = 13u;

uniform uint u_passes;
uniform uvec4 u_pass_flag;

uniform uint u_ignore_bf;

uniform mat4 u_env_mat[4];

in vec2 f_uv[4];
in vec4 f_col;

layout (location = 0) out vec4 o_col;

uniform sampler2D u_texture[4];

uniform uint u_blend[4];
uniform vec4 u_col[4];

uniform uint u_dbg_current_blend;

void main()
{
	// Sample textures
	vec4 t[4];
	for (uint i = 0u; i < u_passes; i++)
	{
		if ((u_pass_flag[i] & MATFLAG_TEXTURED) != 0u)
		{
			if (i == 0u)
				t[i] = texture(u_texture[0], f_uv[i]);
			else if (i == 1u)
				t[i] = texture(u_texture[1], f_uv[i]);
			else if (i == 2u)
				t[i] = texture(u_texture[2], f_uv[i]);
			else if (i == 3u)
				t[i] = texture(u_texture[3], f_uv[i]);
		}
		else
		{
			t[i] = vec4(1.0f);
		}
	}

	// Accumulate first pass
	vec4 r0 = vec4(t[0].rgb * u_col[0].rgb, 0.0f) * 4.0f;
	if ((u_pass_flag[0] & MATFLAG_PASS_IGNORE_VERTEX_ALPHA) != 0u)
		r0.a = t[0].a;
	else
		r0.a = t[0].a * f_col.a * 2.0f;

	r0.rgb *= f_col.rgb;

	// Accumulate remaining passes
	vec4 rl = r0;
	for (uint i = 1u; i < u_passes; i++)
	{
		uint flag = u_pass_flag[i];
		vec4 col = u_col[i];

		// Accumulate pass
		vec4 r = vec4(t[i].rgb * col.rgb, 0.0f) * 4.0f;
		if ((flag & MATFLAG_PASS_IGNORE_VERTEX_ALPHA) != 0u)
			r.a = t[i].a;
		else
			r.a = t[i].a * f_col.a * 2.0f;

		r.rgb *= f_col.rgb;

		// Blend pass
		switch (u_blend[i])
		{
			case vBLEND_MODE_ADD:
				r0.rgb = r.rgb * r.a + r0.rgb;
				break;
			case vBLEND_MODE_ADD_FIXED:
				r0.rgb = r.rgb * col.a + r0.rgb;
				break;
			case vBLEND_MODE_SUBTRACT:
				r0.rgb = r.rgb * -r.a + r0.rgb;
				break;
			case vBLEND_MODE_SUB_FIXED:
				r0.rgb = r.rgb * -col.a + r0.rgb;
				break;
			case vBLEND_MODE_BLEND:
				r0.rgb = mix(r0.rgb, r.rgb, r.a);
				break;
			case vBLEND_MODE_BLEND_FIXED:
				r0.rgb = mix(r0.rgb, r.rgb, col.a);
				break;
			case vBLEND_MODE_MODULATE:
				r0.rgb = r0.rgb * r.a;
				break;
			case vBLEND_MODE_MODULATE_FIXED:
				r0.rgb = r0.rgb * col.a;
				break;
			case vBLEND_MODE_BRIGHTEN:
				r0.rgb = r0.rgb + r0.rgb * r.a;
				break;
			case vBLEND_MODE_BRIGHTEN_FIXED:
				r0.rgb = r0.rgb + r0.rgb * col.a;
				break;
			case vBLEND_MODE_BLEND_PREVIOUS_MASK:
				r0.rgb = mix(r0.rgb, r.rgb, rl.a);
				break;
			case vBLEND_MODE_BLEND_INVERSE_PREVIOUS_MASK:
				r0.rgb = mix(r.rgb, r0.rgb, rl.a);
				break;
			case vBLEND_MODE_GLOSS_MAP:
				break;
		}

		if(u_blend[i] == u_dbg_current_blend) {
		    r0 = vec4(0, 1, 0, 1);
		}

		rl = r;
	}

	o_col = r0;
}