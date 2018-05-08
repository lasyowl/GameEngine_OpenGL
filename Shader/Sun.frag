#version 430 core

layout(location = 0) in vec3 vs_vertex;
layout(location = 2) in vec2 vs_uv;

layout(location = 0) out vec4 finalColor;

uniform sampler2D tex_core;
uniform sampler2D tex_lens[9];
uniform int sequence;
uniform vec2 angle;

void main() {
	vec4 tex;

	if(sequence == 0) {
		tex = texture(tex_core, vs_uv);
		finalColor = tex;
	}
	else {
		tex = texture(tex_lens[sequence - 1], vs_uv);
		finalColor = tex;
		finalColor.w = finalColor.w * (0.7f - 4 * abs(angle.x)) * (0.7f - 4 * abs(angle.y));
		
		if(sequence == 8)
			finalColor.w *= 1.2f;
	}
	if(finalColor.a == 0) {
		discard;
	}
	
}