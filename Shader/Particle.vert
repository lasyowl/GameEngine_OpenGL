#version 430 core

#define MAX_NUM_LIGHT 100

layout(location = 0) in vec4 app_Vertex;
layout(location = 1) in float app_timeRemain;

layout(location = 0) out vec3 vs_position;
layout(location = 1) out float vs_timeRemain;
layout(location = 4) out vec3 vs_posToEye_vector;

uniform mat4 ModelViewProjectionMatrix;

uniform vec3 eye_position;

void main(void){
	gl_Position = ModelViewProjectionMatrix * app_Vertex;
	vs_position = vec3(app_Vertex);
	vs_timeRemain = app_timeRemain;
	vs_posToEye_vector = eye_position - vec3(app_Vertex);
}