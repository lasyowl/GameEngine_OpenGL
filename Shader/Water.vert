#version 430 core

#define MAX_NUM_LIGHT 100

layout(location = 0) in vec4 appVertex;
layout(location = 1) in vec4 appNormal;
layout(location = 2) in vec2 appUv;

layout(location = 0) out vec3 vs_position;
layout(location = 1) out vec3 vs_normal;
layout(location = 2) out vec2 vs_uv;
layout(location = 4) out vec3 posToEye_vector;
layout(location = 6) out float visibility;
layout(location = 8) out vec4 vs_clip;

uniform mat4 ModelViewProjectionMatrix;

uniform vec3 eye_position;

const float density = 0.0035f;
const float gradient = 5.0f;

void main() {
	gl_Position = ModelViewProjectionMatrix * appVertex;
	vs_clip = gl_Position;
	vs_uv = appUv;
	vs_position = vec3(appVertex);
	posToEye_vector = eye_position - appVertex.xyz;

	float posToEye_distance = distance(eye_position, vs_position);
	visibility = exp(-pow((posToEye_distance * density) , gradient));
	visibility = clamp(visibility, 0, 1);
}