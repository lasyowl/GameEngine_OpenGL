#version 430 core

layout(location = 0) out vec4 finalColor;

uniform bool highlight;

void main() {
	if(highlight == false)
		finalColor = vec4(0, 0, 1.0f, 1.0f);
	else finalColor = vec4(0, 1, 0, 1);
}