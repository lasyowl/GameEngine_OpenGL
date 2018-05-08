#version 430 core

layout(location = 10) in vec2 gs_uv0;
layout(location = 11) in vec2 gs_uv1;
layout(location = 12) in float interpolateFactor;

layout(location = 0) out vec4 finalColor;

uniform sampler2D tex_atlas;
uniform bool loop;
uniform float intensity;

void main() {
	vec4 tex0 = texture(tex_atlas, gs_uv0);
	vec4 tex1 = texture(tex_atlas, gs_uv1);
	finalColor = (1 - interpolateFactor) * tex0 + interpolateFactor * tex1;
	
	if(loop == true) {
		finalColor.a *= clamp(intensity, 0, 1);
	}

	if(finalColor.a < 0.1f) {
		discard;
	}
}