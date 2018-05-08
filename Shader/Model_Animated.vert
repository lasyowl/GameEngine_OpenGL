#version 430 core

layout(location = 0) in vec3 appVertex;
layout(location = 1) in vec3 appNormal;
layout(location = 2) in vec2 appUV;
layout(location = 3) in vec4 weight;
layout(location = 4) in uvec4 weightIndex;
layout(location = 5) in mat4 transformMatrix;

layout(location = 0) out vec3 vs_position;
layout(location = 1) out vec3 vs_normal;
layout(location = 2) out vec2 vs_uv;;
layout(location = 4) out vec3 posToEye_vector;
layout(location = 6) out float visibility;
layout(location = 14) out vec3 vs_shadow;

uniform mat4 ModelMatrix;
uniform mat4 ViewProjectionMatrix;
uniform mat3 InvTransposeMatrix;
uniform mat4 BoneMatrix[100];
uniform mat4 ShadowMatrix;

uniform bool useBone = true;
uniform vec3 eye_position;

const float density = 0.0035f;
const float gradient = 5.0f;

const vec4 plane0 = vec4(0, 1, 0, -5);
const vec4 plane1 = vec4(0, -1, 0, 5);

void main() {

	vec4 newPosition;

	vs_uv = appUV;

	if(useBone == true) {
		mat4 WeightMatrix = weight.x * BoneMatrix[weightIndex.x] + weight.y * BoneMatrix[weightIndex.y];
		newPosition = WeightMatrix * vec4(appVertex, 1.0f);
		newPosition /= newPosition.w;

		vs_normal = mat3(WeightMatrix) * appNormal;
	}
	else {
		newPosition = ModelMatrix * vec4(appVertex, 1.0f);
		vs_normal = vec3(ModelMatrix * vec4(appNormal, 0.0f));
	}
	
	gl_ClipDistance[0] = dot(newPosition, plane0);
	gl_ClipDistance[1] = dot(newPosition, plane1);
	gl_ClipDistance[2] = dot(newPosition, plane0);
	
	gl_Position = ViewProjectionMatrix * newPosition;

	posToEye_vector = eye_position - vec3(newPosition);
	vs_position = vec3(newPosition);
	vs_shadow = vec3(ShadowMatrix * newPosition) * 0.5f + 0.5f;
	
	float posToEye_distance = distance(eye_position, vs_position);
	visibility = exp(-pow((posToEye_distance * density) , gradient));
	visibility = clamp(visibility, 0, 1);
}