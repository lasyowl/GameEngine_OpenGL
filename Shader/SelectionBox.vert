#version 430 core

layout(location = 0) in vec3 appVertex;

uniform mat4 ViewProjectionMatrix;
uniform mat4 ModelMatrix;

uniform float boxScale = 1.2f;

void main() {
	gl_Position = ViewProjectionMatrix * ModelMatrix * vec4(appVertex, 1.0f);
}