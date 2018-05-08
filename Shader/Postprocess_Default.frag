#version 430 core

layout(location = 2) in vec2 vs_uv;

layout(location = 0) out vec4 finalColor;

uniform sampler2D tex_framebuffer;
uniform float intensity = 1.0f;
uniform float contrast = 0.1f;

void main() {
	vec4 color_fb = texture(tex_framebuffer, vs_uv);
	//if(vs_uv.x < 0.5f) {
	if(false) {
		finalColor = intensity * color_fb;
	}
	else {
		finalColor.rgb = (color_fb.rgb - 0.5f) * (1.0f + contrast) + 0.5f;
		finalColor.a = color_fb.a;
		finalColor *= intensity;
	}
}