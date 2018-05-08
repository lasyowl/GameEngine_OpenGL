#version 430 core

layout(points, invocations = 2) in;

layout(triangle_strip, max_vertices = 4) out;

layout(location = 5) out flat int color;

uniform mat4 ModelViewProjectionMatrix;
uniform vec3 position;
uniform vec3 eyePosition;
uniform float portion;

void main(void) {

	float dist = distance(position, eyePosition);
	float invDistF = 1.0f / dist;
	vec2 invDist = vec2(6.0f * invDistF, invDistF * 0.7f);
	vec4 pos = ModelViewProjectionMatrix * vec4(position, 1);
	pos /= pos.w;
	
	color = gl_InvocationID;

	//////// Bar background
	if(gl_InvocationID == 0) {
		gl_Position = pos;
		gl_Position.x -= invDist.x;
		gl_Position.y -= invDist.y;
		EmitVertex();
	
		gl_Position = pos;
		gl_Position.x -= invDist.x;
		gl_Position.y += invDist.y;
		EmitVertex();
	
		gl_Position = pos;
		gl_Position.x += invDist.x;
		gl_Position.y -= invDist.y;
		EmitVertex();
	
		gl_Position = pos;
		gl_Position.x += invDist.x;
		gl_Position.y += invDist.y;
		EmitVertex();
	}
	//////// Bar foreground
	else if(gl_InvocationID == 1) {
		invDist.y *= 0.7f;
		invDist.x *= 0.95f;

		gl_Position = pos;
		gl_Position.x -= invDist.x;
		gl_Position.y -= invDist.y;
		gl_Position.z -= 0.000001f;
		EmitVertex();
	
		gl_Position = pos;
		gl_Position.x -= invDist.x;
		gl_Position.y += invDist.y;
		gl_Position.z -= 0.000001f;
		EmitVertex();
	
		gl_Position = pos;
		gl_Position.x += invDist.x * 2 * (portion - 0.5f);
		gl_Position.y -= invDist.y;
		gl_Position.z -= 0.000001f;
		EmitVertex();
	
		gl_Position = pos;
		gl_Position.x += invDist.x * 2 * (portion - 0.5f);
		gl_Position.y += invDist.y;
		gl_Position.z -= 0.000001f;
		EmitVertex();
	}
	
	EndPrimitive();
}