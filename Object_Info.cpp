#include "Object_Info.h"


ObjectInfo::ObjectInfo() : isSelected(false), yOffset(0.0f), HandMatrix(glm::mat4(1)) {

}

ObjectInfo::~ObjectInfo() {

}

void ObjectInfo::IsSelected(bool tf) {
	isSelected = tf;
}