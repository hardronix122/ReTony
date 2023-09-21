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

uniform uint u_passes;
uniform uvec4 u_pass_flag;

uniform uint u_ignore_bf;

uniform mat4 u_env_mat[4];
