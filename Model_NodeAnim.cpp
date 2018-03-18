#include "Model_NodeAnim.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Model_NodeAnim::Model_NodeAnim() {

}

Model_NodeAnim::~Model_NodeAnim() {

}

void Model_NodeAnim::operator = (aiNodeAnim *nodeAnim) {
	name = nodeAnim->mNodeName.data;
	numRkeys = nodeAnim->mNumRotationKeys;
	numSkeys = nodeAnim->mNumScalingKeys;
	numTkeys = nodeAnim->mNumPositionKeys;
	rKeys.resize(numRkeys);
	sKeys.resize(numSkeys);
	tKeys.resize(numTkeys);
	
	for (int i = 0; i < numRkeys; i++) {
		rKeys[i].time = nodeAnim->mRotationKeys[i].mTime;
		rKeys[i].value.x = nodeAnim->mRotationKeys[i].mValue.x;
		rKeys[i].value.y = nodeAnim->mRotationKeys[i].mValue.y;
		rKeys[i].value.z = nodeAnim->mRotationKeys[i].mValue.z;
		rKeys[i].value.w = nodeAnim->mRotationKeys[i].mValue.w;
	}
	for (int i = 0; i < numSkeys; i++) {
		sKeys[i].time = nodeAnim->mScalingKeys[i].mTime;
		sKeys[i].value.x = nodeAnim->mScalingKeys[i].mValue.x;
		sKeys[i].value.y = nodeAnim->mScalingKeys[i].mValue.y;
		sKeys[i].value.z = nodeAnim->mScalingKeys[i].mValue.z;
	}
	for (int i = 0; i < numTkeys; i++) {
		tKeys[i].time = nodeAnim->mPositionKeys[i].mTime;
		tKeys[i].value.x = nodeAnim->mPositionKeys[i].mValue.x;
		tKeys[i].value.y = nodeAnim->mPositionKeys[i].mValue.y;
		tKeys[i].value.z = nodeAnim->mPositionKeys[i].mValue.z;
	}
	for (int i = 0; i < MAX_OBJECT_NUM; i++) {
		recentRKey[i] = rKeys[0].value;
		recentTKey[i] = tKeys[0].value;
		recentSKey[i] = sKeys[0].value;
	}
}

mat4 Model_NodeAnim::CalcTransform(float tick, float duration) {
	this->tick = tick;
	this->duration = duration;
	this->durationInv = 1.0f / duration;

	mat4 answer = CalcT() * CalcR() * CalcS();

	return answer;
}

mat4 Model_NodeAnim::CalcTransform_c(const int &objectNum, Model_NodeAnim *next, const float &ratio) {
	currentObjectNum = objectNum;
	this->nextAnim = next;
	this->ratio = ratio;

	mat4 answer = CalcT_c() * CalcR_c() * CalcS_c();

	return answer;
}

mat4 Model_NodeAnim::CalcR() {
	float rFactor = numRkeys * durationInv;
	glm::uint leftTick = tick * rFactor;
	glm::uint rightTick = (tick + 1) * rFactor;
	float interpolateFactor = tick * rFactor - (float)leftTick;
	aiQuaternion rQuat;
	if (rightTick > numRkeys - 1)
		rightTick = leftTick;
	if (numRkeys > 1) {
		aiQuaternion::Interpolate(rQuat, rKeys[leftTick].value, rKeys[rightTick].value, interpolateFactor);
	}
	else {
		rQuat = rKeys[0].value;
	}

	return transpose(gf.AiToGlm(aiMatrix4x4(rQuat.GetMatrix())));
}

mat4 Model_NodeAnim::CalcS() {
	float sFactor = numSkeys * durationInv;
	uint leftTick = tick * sFactor;
	uint rightTick = (tick + 1) * sFactor;
	float interpolateFactor = tick * sFactor - (float)leftTick;
	if (rightTick > numSkeys - 1)
		rightTick = leftTick;
	if (numSkeys > 1) {
		aiVector3D vector = sKeys[leftTick].value * (1 - interpolateFactor) + sKeys[rightTick].value * interpolateFactor;
		return scale(mat4(1.0f), vec3(vector.x, vector.y, vector.z));
	}
	else return scale(mat4(1.0f), vec3(sKeys[0].value.x, sKeys[0].value.y, sKeys[0].value.z));
}

mat4 Model_NodeAnim::CalcT() {
	float tFactor = numTkeys * durationInv;
	glm::uint leftTick = tick * tFactor;
	glm::uint rightTick = (tick + 1) * tFactor;
	float interpolateFactor = tick * tFactor - (float)leftTick;
	if (rightTick > numTkeys - 1)
		rightTick = leftTick;
	if (numTkeys > 1) {
		aiVector3D vector = tKeys[leftTick].value * (1 - interpolateFactor) + tKeys[rightTick].value * interpolateFactor;
		return translate(mat4(1.0f), vec3(vector.x, vector.y, vector.z));
	}
	else return translate(mat4(1.0f), vec3(tKeys[0].value.x, tKeys[0].value.y, tKeys[0].value.z));
}

mat4 Model_NodeAnim::CalcR_c() {
	aiQuaternion rQuat;
	aiQuaternion::Interpolate(rQuat, recentRKey[currentObjectNum], nextAnim->rKeys[0].value, ratio);
	return transpose(gf.AiToGlm(aiMatrix4x4(rQuat.GetMatrix())));
}

mat4 Model_NodeAnim::CalcS_c() {
	aiVector3D vector = recentSKey[currentObjectNum] * (1 - ratio) + nextAnim->sKeys[0].value * ratio;
	return scale(mat4(1.0f), vec3(vector.x, vector.y, vector.z));
}

mat4 Model_NodeAnim::CalcT_c() {
	aiVector3D vector = recentTKey[currentObjectNum] * (1 - ratio) + nextAnim->tKeys[0].value * ratio;
	return translate(mat4(1.0f), vec3(vector.x, vector.y, vector.z));
}

void Model_NodeAnim::RecordKeys(const int &objectNum, const float &tick, const float &duration) {
	this->tick = tick;
	this->duration = duration;
	this->durationInv = 1.0f / duration;

	float rFactor = numRkeys * durationInv;
	int leftTick = tick * rFactor;
	int rightTick = (tick + 1) * rFactor;
	float interpolateFactor = tick * rFactor - (float)leftTick;
	if (rightTick > numRkeys - 1)
		rightTick = leftTick;
	if (numRkeys > 1) {
		aiQuaternion::Interpolate(recentRKey[objectNum], rKeys[leftTick].value, rKeys[rightTick].value, interpolateFactor);
	}
	else {
		recentRKey[objectNum] = rKeys[0].value;
	}

	float sFactor = numSkeys * durationInv;
	leftTick = tick * sFactor;
	rightTick = (tick + 1) * sFactor;
	interpolateFactor = tick * sFactor - (float)leftTick;
	if (rightTick > numSkeys - 1)
		rightTick = leftTick;
	if (numSkeys > 1) {
		recentSKey[objectNum] = sKeys[leftTick].value * (1 - interpolateFactor) + sKeys[rightTick].value * interpolateFactor;
	}
	else recentSKey[objectNum] = sKeys[0].value;

	float tFactor = numTkeys * durationInv;
	leftTick = tick * tFactor;
	rightTick = (tick + 1) * tFactor;
	interpolateFactor = tick * tFactor - (float)leftTick;
	if (rightTick > numTkeys - 1)
		rightTick = leftTick;
	if (numTkeys > 1) {
		recentTKey[objectNum] = tKeys[leftTick].value * (1 - interpolateFactor) + tKeys[rightTick].value * interpolateFactor;
	}
	else recentTKey[objectNum] = tKeys[0].value;
}