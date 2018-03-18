#ifndef __OBJECT_INFO_H
#define __OBJECT_INFO_H

#include <string>
#include <glm/glm.hpp>

#include "SelectionBox.h"

class Character;

class ObjectInfo {
public:
	ObjectInfo();
	~ObjectInfo();

	void IsSelected(bool tf);

	int modelId;
	glm::vec3 dir;
	float scale;
	glm::vec3 pos;
	glm::mat4 RMatrix;
	glm::vec3 preRot;
	int tag;
	float yOffset;
	glm::mat4 TransformMatrix;
	glm::mat4 SelectionBoxTransformMatrix;
	int charIndex;
	std::string currentAnim;
	bool isAnimating;
	SelectionBox *selectionBox;
	bool isSelected;
	Character *character;

private:
};
#endif