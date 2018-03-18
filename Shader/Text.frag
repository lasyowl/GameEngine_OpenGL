#version 430 core

layout(location = 2) in vec2 vs_uv;

layout(location = 0) out vec4 finalColor;

uniform sampler2DArray tex_text;
uniform int textPage = 5;
uniform vec4 color_text;

const float width = 0.3;
const float edge = 0.1;

void main() {
	vec4 textColor = texture(tex_text, vec3(vs_uv, textPage));

	float distance = 1 - texture(tex_text, vec3(vs_uv, textPage)).a;
	float alpha = 1 - smoothstep(width, width + edge, distance);

	finalColor = vec4(color_text.rgb, textColor.a);
	//finalColor.a = alpha;
}