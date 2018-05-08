#version 430 core

layout(location = 5) in flat int color;

layout(location = 0) out vec4 finalColor;

void main() {
	if(color == 0) {
		finalColor = vec4(0.2f, 1, 0.2f, 1);
	}
	else if(color == 1) {
		finalColor = vec4(0.2f, 0.2f, 1, 1);
	}
	else {
		finalColor = vec4(1,1,1,1);
	}
}