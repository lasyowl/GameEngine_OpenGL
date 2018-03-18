#version 430 core

#define BACKGROUND 0
#define LEFTBAR 1
#define EFFECT 2
#define CURSOR 3

layout(location = 0) in vec3 appVertex;
layout(location = 2) in vec2 appUV;

layout(location = 2) out vec2 vs_uv;

uniform mat4 ViewProjectionMatrix;
uniform float progress;
uniform float originX;
uniform float sizeX;
uniform int marker;

void main() {
	vs_uv = appUV;
	vec3 newVertex = appVertex;

	if(marker == LEFTBAR)
		newVertex.x = (newVertex.x - originX) * progress + originX;
	else if(marker == EFFECT) {
		newVertex.x = (newVertex.x - originX) * progress + originX;
		vs_uv.x = appUV.x * progress;
	}
	else if(marker == CURSOR)
		newVertex.x += sizeX * progress * 0.95f;
	else newVertex.x = (newVertex.x - originX) * min(1, (1 - progress + 0.1f)) + originX + sizeX * max(0, (progress - 0.1f));

	gl_Position = ViewProjectionMatrix * vec4(newVertex, 1);
}