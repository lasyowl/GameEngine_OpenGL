#version 430 core

layout(location = 0) in vec3 appVertex;

uniform mat4 ViewProjectionMatrix;

void main() {
	gl_Position = ViewProjectionMatrix * vec4(appVertex, 1.0f);
	//gl_Position = vec4(0,0,0,1);
}