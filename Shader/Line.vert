#version 430 core

layout(location = 0) in vec3 appVertex;

uniform mat4 ModelViewProjectionMatrix;

void main() {
	gl_Position = ModelViewProjectionMatrix * vec4(appVertex, 1.0f);
}