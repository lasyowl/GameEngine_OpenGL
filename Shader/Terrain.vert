#version 430 core

#define TERRAIN_HEIGHT_ADJUST 100.0f

layout(location = 0) in vec4 appVertex;
layout(location = 1) in vec4 appNormal;
layout(location = 3) in vec3 appTangent;

layout(location = 0) out vec3 vs_position;
layout(location = 1) out vec3 vs_normal;
layout(location = 2) out vec2 vs_uv;
layout(location = 3) out vec3 vs_tangent;
layout(location = 4) out vec3 posToEye_vector;
layout(location = 6) out float visibility;
layout(location = 12) out float slope;
layout(location = 13) out float height;
layout(location = 14) out vec3 vs_shadow;

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ShadowMatrix;

uniform vec3 eye_position;

uniform float heightmapWidthHeight;
uniform float terrainDensity = 20.0f;

const vec4 plane0 = vec4(0, 1, 0, -5 + 1);
const vec4 plane1 = vec4(0, -1, 0, 5);

const float density = 0.00035f;
const float gradient = 9.0f;

void main() {
	gl_ClipDistance[0] = dot(appVertex, plane0);
	gl_ClipDistance[1] = dot(appVertex, plane1);
	gl_ClipDistance[2] = dot(appVertex, plane0);

	gl_Position = ModelViewProjectionMatrix * appVertex;
	vs_position = vec3(appVertex);
	vs_shadow = vec3(ShadowMatrix * appVertex) * 0.5f + 0.5f;
	vs_normal = vec3(appNormal);
	posToEye_vector = eye_position - vec3(appVertex);

	float posToEye_distance = distance(eye_position, vs_position);
	visibility = exp(-pow((posToEye_distance * density) , gradient));
	visibility = clamp(visibility, 0, 1);

	vs_uv = vec2(appVertex.x, -appVertex.z);
	vs_uv = vs_uv * terrainDensity / heightmapWidthHeight;

	slope = 1 - dot(appNormal.xyz, vec3(0,1,0));
	height = appVertex.y / TERRAIN_HEIGHT_ADJUST;
}