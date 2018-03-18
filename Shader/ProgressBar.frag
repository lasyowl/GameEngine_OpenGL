#version 430 core

layout(location = 2) in vec2 vs_uv;

layout(location = 0) out vec4 finalColor;

uniform sampler2D tex_background;
uniform bool highlight;
uniform bool useTexture = false;
uniform vec4 color_background;

void main() {
	finalColor = color_background;
	if(useTexture == true)
		finalColor *= texture(tex_background, vs_uv);
}