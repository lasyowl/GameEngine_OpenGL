#version 430 core

layout(location = 0) in vec3 appVertex;

uniform mat4 ModelViewProjectionMatrix;
uniform float radius;

void main() {
	gl_Position = ModelViewProjectionMatrix * vec4(appVertex * radius, 1.0f);
}