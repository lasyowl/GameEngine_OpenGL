#version 430 core

layout(points) in;

layout(location = 0) in vec4 vs_position[];
layout(location = 2) in float vs_scale[];
layout(location = 4) in float vs_timeRemain[];

layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) out vec3 gs_position;
layout(location = 10) out vec2 gs_uv0;
layout(location = 11) out vec2 gs_uv1;
layout(location = 12) out float interpolateFactor;

uniform mat4 ModelViewProjectionMatrix;
uniform vec3 eyePosition;
uniform int width;
uniform int height;
uniform float scale = 10;
uniform float spriteNum;
uniform float cycleTime;
uniform bool loop;
uniform float intensity;

void main(void) {
	int wh = width * height;
	float timeRemain;
	float sNum;
	if(loop == false) {
		timeRemain = clamp(vs_timeRemain[0], 0, cycleTime);
		if(timeRemain == 0 || timeRemain == cycleTime) return;
		sNum = clamp((1 - timeRemain / cycleTime) * wh, 0, width * height - 1);
	}
	else {
		timeRemain = vs_timeRemain[0];
		sNum = (1 - timeRemain / cycleTime) * wh;
	}

	int intSNum;
	if(sNum > 0) {
		intSNum = int(sNum);
	}
	else {
		intSNum = int(floor(sNum));
	}

	interpolateFactor = fract(sNum);
	float invWidth = 1 / float(width);
	float invHeight = 1.0f / float(height);
	float x0 = float(intSNum % width) * invWidth;
	float y0 = 1 - float(intSNum / width) * invHeight - invHeight;
	float x1 = float((intSNum + 1) % width) * invWidth;
	float y1 = 1 - float((intSNum + 1) / width) * invHeight - invHeight;

	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(eyePosition - pos);
	vec3 upVector = vec3(0, 1, 0);
	vec3 right = cross(upVector, toCamera);

	pos -= 0.5 * scale * right;
	gl_Position = ModelViewProjectionMatrix * vec4(pos, 1);
	gs_uv0 = vec2(x0, y0);
	gs_uv1 = vec2(x1, y1);
	EmitVertex();
	
	pos.y += scale;
	gl_Position = ModelViewProjectionMatrix * vec4(pos, 1);
	gs_uv0 = vec2(x0, y0 + invHeight);
	gs_uv1 = vec2(x1, y1 + invHeight);
	EmitVertex();
	
	pos.y -= scale;
	pos += scale * right;
	gl_Position = ModelViewProjectionMatrix * vec4(pos, 1);
	gs_uv0 = vec2(x0 + invWidth, y0);
	gs_uv1 = vec2(x1 + invWidth, y1);
	EmitVertex();
	
	pos.y += scale;
	gl_Position = ModelViewProjectionMatrix * vec4(pos, 1);
	gs_uv0 = vec2(x0 + invWidth, y0 + invHeight);
	gs_uv1 = vec2(x1 + invWidth, y1 + invHeight);
	EmitVertex();

	EndPrimitive();
}