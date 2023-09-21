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

		if(i <= u_max_passes) {
			uint blend_mode = (uint(0xFFFFFF) & u_reg_alpha[i]);
            uint fixed_alpha_int = (uint(0x000000FF) & u_reg_alpha[i]);
            float fixed_alpha = fixed_alpha_int / 128.0f;

		    if(blend_mode == BLEND_MODE_DIFFUSE) {
		        accum = texel;
		    } else if(blend_mode == BLEND_MODE_ADD) {
		        accum = texel * texel + accum;
		    } else if(blend_mode == BLEND_MODE_ADD_FIXED) {
		        accum = texel * fixed_alpha + accum;
		    } else if(blend_mode == BLEND_MODE_SUBTRACT) {
		    	accum = texel * texel + accum;
		    } else if(blend_mode == BLEND_MODE_SUBTRACT_FIXED) {
                accum = texel * fixed_alpha + accum;
		    } else if(blend_mode == BLEND_MODE_BLEND) {
		        accum = mix(accum, texel, texel.a);
		    } else if(blend_mode == BLEND_MODE_BLEND_FIXED) {
                accum = mix(accum, texel, fixed_alpha);
		    } else if(blend_mode == BLEND_MODE_MODULATE) {
		        accum = accum * texel;
		    } else if(blend_mode == BLEND_MODE_MODULATE_FIXED) {
		        accum = accum * fixed_alpha;
		    } else if(blend_mode == BLEND_MODE_BRIGHTEN) {
		        accum = accum * texel + accum;
		    } else if(blend_mode == BLEND_MODE_BRIGHTEN_FIXED) {
		        accum = accum * fixed_alpha + accum;
		    } else if(blend_mode == BLEND_MODE_GLOSS_MAP) {
		        accum = vec4(1, 0, 1, 1);
		    } else if(blend_mode == BLEND_MODE_BLEND_PREVIOUS_MASK) {
		        accum = (texel - accum) * accum + accum;
		    } else if(blend_mode == BLEND_MODE_BLEND_INVERSE_PREVIOUS_MASK) {
                accum = (accum - texel) * accum + texel;
		    } else {
		        accum = vec4(1, 0, 0, 1);
		    }

            if(u_active_blend > -1) {
                if(int(blend_mode) == u_active_blend) {
                    accum = vec4(0, 1, 0, 1);
                }
            }
		}
	}
	o_col = accum;
}
