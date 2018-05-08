#version 430 core

layout(location = 0) in vec3 app_vertex;
layout(location = 1) in vec3 app_direction;
layout(location = 2) in float app_scale;
layout(location = 3) in float app_velocity;
layout(location = 4) in float app_timeRemain;

layout(location = 0) out vec4 vs_position;
layout(location = 2) out float vs_scale;
layout(location = 4) out float vs_timeRemain;

void main(void) {
	gl_Position = vec4(app_vertex, 1);
	vs_scale = app_scale;
	vs_timeRemain = app_timeRemain;
}