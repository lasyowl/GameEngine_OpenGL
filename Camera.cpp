#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Character.h"

using namespace glm;

Camera::Camera() : pov(THIRD_PERSON), seq2Angle(0.0f), offsetLevel(-100.0f) {
	direction = vec3(0.0f, 0.0f, -1.0f);
	position = vec3(0, 1, 0);
}

Camera::Camera(const vec2 &viewport) : pov(THIRD_PERSON), seq2Angle(0.0f), offsetLevel(-100.0f) {
	this->viewport = viewport;
	direction = vec3(0.0f, 0.0f, -1.0f);
	position = vec3(0, 1, 0);
}

Camera::~Camera() {

}

void Camera::SetViewport(const vec2 &viewport) {
	this->viewport = viewport;
}

void Camera::UpdatePos() {
	if (pov == SKY_VIEW) {
		position = vec3(128.0f, 100.0f, -128.0f);
	}
	else if (pov == THIRD_PERSON) {
		position = character->GetPos();
		position.y += 0.5f;
	}
	else if (pov == STAND_ALONE) {
		
	}
}

void Camera::UpdateDir() {
	direction = mat3(RMatrix) * vec3(0, 0, -1);
}

void Camera::SetReference(Character *character) {
	this->character = character;
}

void Camera::AdjOffset(const float &level) {
	offsetLevel += level;
	offsetLevel = min(0, offsetLevel);
}

vec3 Camera::GetPos() {
	return position;
}

vec3 Camera::GetOffset() {
	return offsetLevel * direction;
}

vec3 Camera::GetDir() {
	return direction;
}

mat4 Camera::GetRMatrix() {
	return RMatrix;
}

mat4 Camera::GetProjMatrix() {
	return perspective(radians(60.0f), viewport.x / viewport.y, 0.5f, 2000.0f);
}

mat4 Camera::GetViewMatrix() {
	return lookAt(GetPos() + GetOffset(), GetPos() + GetOffset() + GetDir(), vec3(0.0f, 1.0f, 0.0f));
}

float Camera::GetOffsetLevel() {
	return offsetLevel;
}

void Camera::SetRMatrix(const mat4 &RMatrix) {
	this->RMatrix = RMatrix * this->RMatrix;
}

void Camera::SetDir(const vec3 &dir) {
	direction = dir;
}

void Camera::Move(const glm::vec3 &dir, const float &speed) {
	position += speed * dir;
}

vec3 Camera::GetRightDir() {
	vec3 proj = direction;
	proj.y = 0;
	normalize(proj);
	vec3 rightDir = cross(proj, vec3(0, 1, 0));

	return rightDir;
}

void Camera::Rotate(const int &rotSequence, const float &angle) {
	mat4 RMatrix;
	switch (rotSequence) {
	case SEQ_ONE:
		RMatrix = rotate(mat4(1.0f), radians(angle), vec3(0.0f, 1.0f, 0.0f));
		break;
	case SEQ_TWO:
		if (seq2Angle + angle > 89.9f) {
			RMatrix = rotate(mat4(1.0f), radians(89.9f - seq2Angle), GetRightDir());
			seq2Angle = 89.9f;
		}
		else if (seq2Angle + angle < -89.9f) {
			RMatrix = rotate(mat4(1.0f), radians(-89.9f - seq2Angle), GetRightDir());
			seq2Angle = -89.9f;
		}
		else { 
			RMatrix = rotate(mat4(1.0f), radians(angle), GetRightDir()); 
			seq2Angle += angle;
		}
		break;
	default:
		break;
	}
	direction = normalize(mat3(RMatrix) * direction);
}

void Camera::SetPov(int pov) {
	this->pov = pov;
}

void Camera::SwitchPov() {
	pov++;
	if (pov == 3)
		pov = 0;
}

vec3 Camera::GetMouseWorldCoord(const vec2 &mouseCoord) {
	vec2 windowCoord = 2.0f * mouseCoord / viewport - 1.0f;
	windowCoord.y = - windowCoord.y;
	vec4 clipCoord = vec4(windowCoord, -1, 1);
	vec4 eyeCoord = inverse(GetProjMatrix()) * clipCoord;
	eyeCoord.w = 0;
	vec3 worldCoord = normalize(vec3(inverse(GetViewMatrix()) * eyeCoord));

	//printf("%f %f %f\n", worldCoord.x, worldCoord.y, worldCoord.z);

	return worldCoord;
}