#version 430 core

layout(location = 0) in vec3 appVertex;
layout(location = 1) in vec3 appNormal;
layout(location = 2) in vec2 appUV;

layout(location = 0) out vec3 vs_position;
layout(location = 1) out vec3 vs_normal;
layout(location = 2) out vec2 vs_uv;
layout(location = 4) out vec3 posToEye_vector;
layout(location = 6) out float visibility;

uniform mat4 ModelMatrix;
uniform mat4 ViewProjectionMatrix;

uniform vec3 eye_position;

const float density = 0.0035f;
const float gradient = 5.0f;

const vec4 plane0 = vec4(0, 1, 0, -5);
const vec4 plane1 = vec4(0, -1, 0, 4.8f);

void main() {

	vec4 newPosition = ModelMatrix * vec4(appVertex, 1.0f);
	
	gl_ClipDistance[0] = dot(newPosition, plane0);
	gl_ClipDistance[1] = dot(newPosition, plane1);
	gl_ClipDistance[2] = dot(newPosition, plane0);

	gl_Position = ViewProjectionMatrix * newPosition;
	vs_position = vec3(ModelMatrix * vec4(appVertex, 1.0f));
	vs_normal = mat3(ModelMatrix) * appNormal;
	vs_uv = appUV;
	posToEye_vector = eye_position - vec3(appVertex);

	float posToEye_distance = distance(eye_position, vs_position);
	visibility = exp(-pow((posToEye_distance * density) , gradient));
	visibility = clamp(visibility, 0, 1);
}