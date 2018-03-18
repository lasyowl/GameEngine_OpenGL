#version 430 core

layout(location = 0) out vec4 finalColor;

uniform vec3 color_line = vec3(0, 0, 1);

void main(){

	finalColor = vec4(color_line, 1);
}