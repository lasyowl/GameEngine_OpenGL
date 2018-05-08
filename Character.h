#ifndef __CHARACTER_H
#define __CHARACTER_H

#include <list>
#include <string>
#include <glm/glm.hpp>

#include "Render_Models_Animated.h"
#include "Render_Models_Static.h"
#include "Mesh_Terrain.h"
#include "ModelTransform_Loader.h"
#include "ObjectFinder.h"
#include "Collider_Sphere.h"

#define CHAR_ACTIV_FAIL false
#define CHAR_ACTIV_SUCCESS true

class PathFinder_Node;

enum Tag { PLAYER, ENEMY, COPLAYER };
enum RotSequence { SEQ_ONE, SEQ_TWO };
enum State { STATE_IDLE, STATE_WALK, STATE_RUN, STATE_ATTACK, STATE_JUMP, STATE_MOVE, STATE_CASTING, STATE_DEATH };

class Character {
public:
	Character();
	virtual ~Character();

	bool operator == (const Character &rightHand);

	void Initiate(const ObjectInfo &objectInfo, Render_Models_Animated *models_animated, Mesh_Terrain *terrain);
	void Move(const glm::vec3 &dir, const bool &clearPath = true);
	void Rotate(const int &rotSequence, const float &angle);
	void SetState(const int &state);
	void SetDir(const glm::vec3 &dir);
	void SetXZPos(const float &x, const float &z);
	void SetPos(const glm::vec3 &pos);
	void GenPreRotMatrix();
	glm::vec3 GetPos();
	glm::vec3 GetPos_HUD();
	glm::vec3 GetDir();
	glm::vec3 GetRightDir();
	glm::mat4 GetRMatrix();
	float GetScale();
	int GetTag();
	glm::mat4 GetTransformMatrix();
	glm::mat4 GetSelectBoxTransformMatrix();
	bool Activate();
	void Update();
	void MaintainAnim(const bool &maintain);
	bool IsJumping();
	void ReservePath(std::vector<PathFinder_Node*> &reservedPath);
	void ClearPath();
	void Damage(const float &damage);

	bool calcGravity;
	int animState;
	int animDuration;
	float speed;
	bool isRunning;
	float scale;
	glm::vec3 position;
	glm::vec3 preRotation;
	std::list<ObjectInfo>::iterator iter;
	std::vector<PathFinder_Node*> reservedPath;
	int pathSeq;

	ObjectFinder *objectFinder;
	Collider_Sphere collider;
	float recogDist;
	float hitPoint_current;
	float hitPoint_max;
	bool isDying;

protected:
	void SetObjectInfo(const ObjectInfo &objectInfo);
	void SetModelId(const int &modelId);
	void SetTerrain(Mesh_Terrain *terrain);
	void SetScale(const float &scale);
	void SetRMatrix(const glm::mat4 &RMatrix);
	void SetPreRot(const glm::vec3 &preRot);
	void SetTag(const int &tag);
	void SetYOffset(const float &yOffset);
	int PlayAnim(const std::string &animName, const int &playCount, const bool &loopAnim = false);
	void ReservedMove();

	Mesh_Terrain *mesh_terrain;

	bool isInitiated;
	bool isInherited;

	int modelId;
	glm::vec3 direction;
	glm::vec3 dir_origin;
	float seq2Angle;
	int tag;
	glm::mat4 RMatrix;
	glm::mat4 PreRotMatrix;
	float yOffset;
	float yJumpOffset;
	float ySpeed;
	const float yForce;
	bool maintainAnim;
	bool isJumping;
	ObjectInfo objectInfo;
	

private:
	void SetRenderer(Render_Models_Animated *renderer);

	Render_Models_Animated *render_animated;

};
#endif