#version 430 core

#define GBLUR_COEF 21
#define SEQ_BRIGHT 0
#define SEQ_VERT 1
#define SEQ_HORIZ 2
#define SEQ_FINAL 3

layout(location = 2) in vec2 vs_uv;

layout(location = 0) out vec4 finalColor;

uniform sampler2D tex_framebuffer;
uniform sampler2D tex_gblur;
uniform float intensity = 1.0f;
uniform int state_gblur = 1;

void main() {
	vec4 color_fb = texture(tex_framebuffer, vs_uv);
	
	vec2 coord[GBLUR_COEF];

	vec3 col = vec3(0,0,0);

	//11 : float g_coef[GBLUR_COEF] = float[GBLUR_COEF](0.066414,0.079465,0.091364,0.100939,0.107159,0.109317,0.107159,0.100939,0.091364,0.079465,0.066414);
	float g_coef[GBLUR_COEF] = float[GBLUR_COEF](0.011254,0.016436,0.023066,0.031105,0.040306,0.050187,0.060049,0.069041,0.076276,0.080977,0.082607,0.080977,0.076276,0.069041,0.060049,0.050187,0.040306,0.031105,0.023066,0.016436,0.011254);

	float brightness = (color_fb.r * 0.2126 + color_fb.g * 0.7152 + color_fb.b * 0.0722);

	switch(state_gblur) {
	case SEQ_BRIGHT:
		if(brightness > 0.3f) {
			finalColor.rgb = color_fb.rgb;
		}
		else {
			finalColor.rgb = vec3(0, 0, 0);
		}
		finalColor.a = 1;
		break;
	case SEQ_VERT:
		for(int i = -(GBLUR_COEF - 1) / 2; i < (GBLUR_COEF - 1) / 2; i++){
			coord[i + (GBLUR_COEF - 1) / 2] = vs_uv + vec2(i / 800.0f, 0);
			col += g_coef[i + (GBLUR_COEF - 1) / 2] * texture(tex_framebuffer, coord[i + (GBLUR_COEF - 1) / 2]).xyz;
		}
		finalColor = vec4(col, 1);
		break;
	case SEQ_HORIZ:
		for(int i = -(GBLUR_COEF - 1) / 2; i < (GBLUR_COEF - 1) / 2; i++){
			coord[i + (GBLUR_COEF - 1) / 2] = vs_uv + vec2(0, i / 800.0f);
			col += g_coef[i + (GBLUR_COEF - 1) / 2] * texture(tex_framebuffer, coord[i + (GBLUR_COEF - 1) / 2]).xyz;
		}
		finalColor = vec4(col, 1);
		break;
	case SEQ_FINAL:
		vec4 color_gblur = texture(tex_gblur, vs_uv);
		finalColor.xyz = (color_fb.xyz + color_gblur.xyz) * 0.6f;
		//finalColor.xyz = (color_gblur.xyz);
		finalColor.w = 1;
		break;
	default:
		break;
	}
}