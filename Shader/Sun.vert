#version 430 core

layout(location = 0) in vec3 appVertex;
layout(location = 2) in vec2 appUV;

layout(location = 0) out vec3 vs_vertex;
layout(location = 2) out vec2 vs_uv;

uniform mat4 ModelViewProjectionMatrix;
uniform int sequence;
uniform vec2 angle;

const float scale[9] = { 32, 8, 16, 3, 16, 4, 8, 1.3, 32 };
const float trans[9] = { 6, 2, 1.9, 0.4, 0.9, 1.4, 1, 1.05, 0.9 };

void main() {
	if(sequence == 0) {
		gl_Position = ModelViewProjectionMatrix * vec4(appVertex, 1.0f);
	}
	else {
		gl_Position = ModelViewProjectionMatrix * vec4(appVertex / scale[sequence - 1], 1.0f);
		gl_Position /= gl_Position.w;
		gl_Position.z -= sequence * 0.1f;
		gl_Position.x -= angle.x * sequence * 1.8f * trans[sequence - 1];
		gl_Position.y -= angle.y * sequence * 1.8f * trans[sequence - 1];
	}

	vs_uv = appUV;
}