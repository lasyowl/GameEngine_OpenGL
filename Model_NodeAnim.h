#ifndef __MODEL_NODEANIM_H
#define __MODEL_NODEANIM_H

#include <vector>
#include <string>
#include <assimp/quaternion.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "GlobalFunctions.h"
#include "Model_Node.h"

class Model_NodeAnim {
public:
	struct Rot {
		float time;
		aiQuaternion value;
	};
	struct Pos {
		float time;
		aiVector3D value;
	};
	struct Scale {
		float time;
		aiVector3D value;
	};

	Model_NodeAnim();
	~Model_NodeAnim();

	glm::mat4 CalcTransform(float tick, float duration);
	glm::mat4 CalcTransform_c(const int &ObjectNum, Model_NodeAnim *next, const float &ratio);
	void RecordKeys(const int &objectNum, const float &tick, const float &duration);

	void operator = (aiNodeAnim *nodeAnim);

	std::string name;
	int numRkeys;
	int numSkeys;
	int numTkeys;
	std::vector<Rot> rKeys;
	std::vector<Pos> tKeys;
	std::vector<Scale> sKeys;
	aiQuaternion recentRKey[MAX_OBJECT_NUM];
	aiVector3D recentTKey[MAX_OBJECT_NUM];
	aiVector3D recentSKey[MAX_OBJECT_NUM];

private:
	glm::mat4 CalcR();
	glm::mat4 CalcS();
	glm::mat4 CalcT();
	glm::mat4 CalcR_c();
	glm::mat4 CalcS_c();
	glm::mat4 CalcT_c();

	Model_NodeAnim *nextAnim;

	GlobalFunctions gf;
	float tick;
	float duration;
	float durationInv;
	float ratio;
	int currentObjectNum;
};

#endif