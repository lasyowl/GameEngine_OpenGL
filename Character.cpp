#include <glm/gtc/matrix_transform.hpp>

#include "GlobalFunctions.h"
#include "Character.h"
#include "Render_Models_Animated.h"

using namespace glm;
using namespace std;

Character::Character() : isInitiated(false), preRotation(vec3(0,0,0)), seq2Angle(0.0f), animState(STATE_MOVE), maintainAnim(false), 
yOffset(0.0f), yJumpOffset(0.0f), ySpeed(0.0f), yForce(0.002f), isJumping(false), isInherited(false), calcGravity(false) {
	dir_origin = vec3(0, 0, -1);
}

Character::~Character() {
	if (isInitiated == true && isInherited == false)
		render_animated->DeleteObject(iter);
}

bool Character::operator == (const Character &rightHand) {
	if (&rightHand == this)
		return true;
	else return false;
}

void Character::Initiate(const ObjectInfo &objectInfo, Render_Models_Animated *models_animated, Mesh_Terrain *terrain) {
	SetRenderer(models_animated);
	SetTerrain(terrain);
	SetObjectInfo(objectInfo);
	SetPos(objectInfo.pos);
	SetDir(objectInfo.dir);
	SetScale(objectInfo.scale);
	SetRMatrix(objectInfo.RMatrix);
	SetPreRot(objectInfo.preRot);
	SetYOffset(objectInfo.yOffset);
	SetTag(objectInfo.tag);
	SetModelId(objectInfo.modelId);
	GenPreRotMatrix();
}

bool Character::Activate() {
	iter = render_animated->AddObject(objectInfo);
	if (iter == render_animated->IsFull()) {
		printf("Activation fail\n");
		return CHAR_ACTIV_FAIL;
	}
	iter->isAnimating = false;
	isInitiated = true;
	direction = dir_origin;
	return CHAR_ACTIV_SUCCESS;
}

void Character::Update() {
	position.y += ySpeed;
	float height;
	if (calcGravity == true) {
		height = mesh_terrain->GetHeight(position);
		if (height < position.y) {
			ySpeed -= yForce;
		}
		else {
			ySpeed = 0.0f;
			isJumping = false;
		}
		position.y = glm::max(height, position.y);
	}

	iter->TransformMatrix = GetTransformMatrix();
	iter->SelectionBoxTransformMatrix = GetSelectBoxTransformMatrix();
	iter->pos = position;
	iter->scale = this->scale;
	iter->preRot = this->preRotation;
}

void Character::GenPreRotMatrix() {
	PreRotMatrix = rotate(mat4(1.0f), radians(preRotation.x), vec3(1.0f, 0.0f, 0.0f));
	PreRotMatrix = rotate(PreRotMatrix, radians(preRotation.y), vec3(0.0f, 1.0f, 0.0f));
	PreRotMatrix = rotate(PreRotMatrix, radians(preRotation.z), vec3(0.0f, 0.0f, 1.0f));
}

void Character::SetObjectInfo(const ObjectInfo &objectInfo) {
	this->objectInfo = objectInfo;
}

void Character::SetPos(const vec3 &pos) {
	this->position = pos;
	//this->position.y = mesh_terrain->GetHeight(this->position);
}

void Character::SetYOffset(const float &yOffset) {
	this->yOffset = yOffset;
}

void Character::SetModelId(const int &modelId) {
	this->modelId = modelId;
}

void Character::SetDir(const vec3 &dir) {
	this->direction = dir;
}

void Character::SetXZPos(const float &x, const float &z) {
	this->position.x = x;
	this->position.z = z;
}

void Character::SetScale(const float &scale) {
	this->scale = scale;
}

void Character::SetRMatrix(const mat4 &RMatrix) {
	this->RMatrix = RMatrix;
}

void Character::SetPreRot(const vec3 &preRot) {
	this->preRotation = preRot;
}

void Character::SetTag(const int &tag) {
	this->tag = tag;
	if (tag == 0) {
		calcGravity = true;
	}
}

vec3 Character::GetPos() {
	return position;
}

vec3 Character::GetDir() {
	return direction;
}

vec3 Character::GetRightDir() {
	vec3 proj = direction;
	proj.y = 0;
	normalize(proj);
	vec3 rightDir = cross(proj, vec3(0, 1, 0));
	
	return rightDir;
}

void Character::Move(const vec3 &dir, const float &speed) {
	if (calcGravity == true) {
		// stick on terrain
		vec3 newDir = dir;
		newDir.y = 0;
		newDir = normalize(newDir);
		position += speed * newDir;
		position.y = glm::max(mesh_terrain->GetHeight(position), position.y);
	}
	else {
		// fly
		position += speed * dir;
	}
}

void Character::Rotate(const int &rotSequence, const float &angle) {
	mat4 RMatrix;
	switch (rotSequence) {
		case SEQ_ONE:
			RMatrix = rotate(mat4(1.0f), radians(angle), vec3(0.0f, 1.0f, 0.0f));
			break;
		case SEQ_TWO:
			if (seq2Angle + angle > 89.9f)
				RMatrix = rotate(mat4(1.0f), radians(89.9f - seq2Angle), GetRightDir());
			else if (seq2Angle + angle < -89.9f)
				RMatrix = rotate(mat4(1.0f), radians(-89.9f - seq2Angle), GetRightDir());
			else RMatrix = rotate(mat4(1.0f), radians(angle), GetRightDir());
			break;
		default:
			break;
	}
	direction = normalize(mat3(RMatrix) * direction);
}

mat4 Character::GetRMatrix() {
	vec3 proj = normalize(vec3(-direction.z, 0, -direction.x));
	float yRadian = acos(proj.x);
	if (direction.x > 0)
		yRadian = -yRadian;

	// if fly -> activate
	/*float xRadian = acos(sqrt(pow(direction.x, 2) + pow(direction.z, 2)));
	if (direction.y < 0)
		xRadian = -xRadian;
	mat4 RMat = rotate(mat4(1), xRadian, GetRightDir());*/
	mat4 RMat = rotate(mat4(1), 0.0f, GetRightDir());

	RMat = rotate(RMat, yRadian, vec3(0, 1, 0));
	return RMat;
}

float Character::GetScale() {
	return this->scale;
}

int Character::GetTag() {
	return this->tag;
}

mat4 Character::GetTransformMatrix() {
	mat4 TransformMatrix = translate(mat4(1.0f), position + vec3(0, yOffset, 0));
	TransformMatrix = TransformMatrix * GetRMatrix() * PreRotMatrix * glm::scale(mat4(1.0f), vec3(scale));
	
	return TransformMatrix;
}

mat4 Character::GetSelectBoxTransformMatrix() {
	mat4 TransformMatrix = translate(mat4(1.0f), position + vec3(0, yOffset, 0));
	TransformMatrix = TransformMatrix * PreRotMatrix * glm::scale(mat4(1.0f), vec3(scale));

	return TransformMatrix;
}

void Character::SetRenderer(Render_Models_Animated *renderer) {
	render_animated = renderer;
}

void Character::SetTerrain(Mesh_Terrain *terrain) {
	mesh_terrain = terrain;
}

void Character::PlayAnim(const string &animName, const int &playCount) {
	iter->currentAnim = animName;
	render_animated->PlayAnim(iter, playCount);
}

void Character::MaintainAnim(const bool &maintain) {
	maintainAnim = maintain;
}

void Character::SetState(const int &state) {
	animState = state;
	switch (state) {
		case STATE_IDLE:
			PlayAnim("Idle", 100);
			break;
		case STATE_WALK:
			PlayAnim("Walk", 100);
			animState = STATE_MOVE;
			break;
		case STATE_RUN:
			PlayAnim("Run", 100);
			animState = STATE_MOVE;
			break;
		case STATE_ATTACK:
			PlayAnim("Attack1", 1);
			break;
		case STATE_JUMP:
			ySpeed = 0.1f;
			PlayAnim("Jump", 1);
			isJumping = true;
			break;
		default:
			break;
	}
}

bool Character::IsJumping() {
	return isJumping;
}