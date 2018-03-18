#version 430 core

layout(location = 2) in vec2 vs_uv;

layout(location = 0) out vec4 finalColor;

uniform sampler2DArray tex_background_a;
uniform sampler2D tex_background;
uniform sampler2D tex_border;
uniform bool useBorder;
uniform bool highlight;
uniform bool useTexture = false;
uniform vec4 color_background;
uniform float intensity = 1.0f;
uniform int texPackNum = -1;

void main() {
	finalColor = color_background;
	if(useTexture == true) {
		if(texPackNum >= 0) {
			finalColor *= texture(tex_background_a, vec3(vs_uv, texPackNum));
		}
		else {
			finalColor *= texture(tex_background, vs_uv);
		}
	}

	if(highlight == true)
		finalColor *= 1.5f;

	finalColor.xyz *= intensity;
}