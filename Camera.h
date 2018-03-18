#ifndef __CAMERA_H
#define __CAMERA_H

#include <glm/glm.hpp>
#include "GlobalFunctions.h"

class Character;

#define CAMERA_OFFSET 5.0f

enum POV { FIRST_PERSON, THIRD_PERSON, SKY_VIEW, STAND_ALONE };

class Camera {
public:
	Camera();
	Camera(const glm::vec2 &viewport);
	~Camera();

	void SetViewport(const glm::vec2 &viewport);
	void UpdatePos();
	void UpdateDir();
	void SetReference(Character *character);
	void AdjOffset(const float &level);
	glm::vec3 GetPos();
	glm::vec3 GetOffset();
	glm::vec3 GetDir();
	glm::vec3 GetRightDir();
	glm::mat4 GetProjMatrix();
	glm::mat4 GetViewMatrix();
	float GetOffsetLevel();
	void Rotate(const int &rotSequence, const float &angle);
	void SetRMatrix(const glm::mat4 &RMatrix);
	void SetDir(const glm::vec3 &dir);
	void Move(const glm::vec3 &dir, const float &speed);
	glm::mat4 GetRMatrix();
	// * Flag : FIRST_PERSON, THIRD_PERSON
	void SetPov(int pov);
	void SwitchPov();
	glm::vec3 GetMouseWorldCoord(const glm::vec2 &mouseCoord);

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::mat4 RMatrix;
	Character *character;

	float seq2Angle;
	int pov;
	float offsetLevel;
	glm::vec2 viewport;

	GlobalFunctions gf;
};
#endif