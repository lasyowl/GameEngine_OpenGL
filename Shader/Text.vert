#version 430 core

layout(location = 0) in vec3 appVertex;
layout(location = 2) in vec2 appUV;

layout(location = 2) out vec2 vs_uv;

uniform mat4 ViewProjectionMatrix;
uniform vec2 textOffset;
uniform vec2 boxOffset;
uniform float fontSize = 1.0f;

void main() {
	vs_uv = appUV;
	vec3 newVertex = appVertex + vec3(textOffset, 0);
	newVertex = fontSize * newVertex + vec3(boxOffset, 0);
	gl_Position = ViewProjectionMatrix * vec4(newVertex, 1);
}