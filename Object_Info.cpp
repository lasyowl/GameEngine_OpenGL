#include "Object_Info.h"


ObjectInfo::ObjectInfo() : isSelected(false) {

}

ObjectInfo::~ObjectInfo() {

}

void ObjectInfo::IsSelected(bool tf) {
	isSelected = tf;
}