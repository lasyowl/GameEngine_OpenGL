#include "Model_Bone.h"

Model_Bone::Model_Bone() {

}

Model_Bone::~Model_Bone() {

}

Model_Bone::Model_Bone(std::string name, int index) {
	this->name = name;
	this->index = index;
}