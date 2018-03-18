#version 430 core

layout(location = 0) in vec3 appVertex;
layout(location = 2) in vec2 appUV;

layout(location = 2) out vec2 vs_uv;

uniform mat4 ModelViewProjectionMatrix;

void main() {
	vs_uv = appUV;
	gl_Position = ModelViewProjectionMatrix * vec4(appVertex, 1);
}