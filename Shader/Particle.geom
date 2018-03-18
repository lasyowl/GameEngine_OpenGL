#version 430 core

#define MAX_NUM_LIGHT 100

layout(points) in;

layout(location = 0) in vec3 vs_position[];
layout(location = 1) in float vs_timeRemain[];
layout(location = 4) in vec3 vs_posToEye_vector[];

layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) out vec3 gs_position;
layout(location = 1) out float gs_timeRemain;
layout(location = 2) out vec2 gs_texCoord0;
layout(location = 3) out vec2 gs_texCoord1;
layout(location = 4) out vec3 gs_posToEye_vector;
layout(location = 5) out float blendFactor;

vec2 CalcBlockCoord(float blockNum) {
	ivec2 texCoord_int;
	texCoord_int.y = int(3.0f - floor(blockNum * 0.25f));
	texCoord_int.x = int(blockNum - texCoord_int.y * 4.0f);
	return vec2(float(texCoord_int.x) / 4.0f, float(texCoord_int.y) / 4.0f);
}

void main(void) {
	float blockNum = 16.0f - vs_timeRemain[0] * 0.16f;
	blendFactor = blockNum - floor(blockNum);
	vec2 texCoord0 = CalcBlockCoord(blockNum);
	vec2 texCoord1 = CalcBlockCoord(blockNum + 1.0f);


	gl_Position = gl_in[0].gl_Position + vec4(-0.1f, -0.1f, 0.0f, 0.0f);
	gs_position = vs_position[0];
	gs_texCoord0 = vec2(texCoord0.x, texCoord0.y);
	gs_texCoord1 = vec2(texCoord1.x, texCoord1.y);
	gs_timeRemain = vs_timeRemain[0];
	gs_posToEye_vector = vs_posToEye_vector[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(-0.1f, 0.1f, 0.0f, 0.0f);
	gs_position = vs_position[0];
	gs_texCoord0 = vec2(texCoord0.x, texCoord0.y + 0.25f);
	gs_texCoord1 = vec2(texCoord1.x, texCoord1.y + 0.25f);
	gs_timeRemain = vs_timeRemain[0];
	gs_posToEye_vector = vs_posToEye_vector[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.1f, -0.1f, 0.0f, 0.0f);
	gs_position = vs_position[0];
	gs_texCoord0 = vec2(texCoord0.x + 0.25f, texCoord0.y);
	gs_texCoord1 = vec2(texCoord1.x + 0.25f, texCoord1.y);
	gs_timeRemain = vs_timeRemain[0];
	gs_posToEye_vector = vs_posToEye_vector[0];
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.1f, 0.1f, 0.0f, 0.0f);
	gs_position = vs_position[0];
	gs_texCoord0 = vec2(texCoord0.x + 0.25f, texCoord0.y + 0.25f);
	gs_texCoord1 = vec2(texCoord1.x + 0.25f, texCoord1.y + 0.25f);
	gs_timeRemain = vs_timeRemain[0];
	gs_posToEye_vector = vs_posToEye_vector[0];
	EmitVertex();

	EndPrimitive();
}