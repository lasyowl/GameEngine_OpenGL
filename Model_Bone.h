#ifndef __MODEL_BONE_H
#define __MODEL_BONE_H

#include <string>

class Model_Bone {
public:
	Model_Bone();
	Model_Bone(std::string name, int index);
	~Model_Bone();

	std::string name;
	int index;

private:
	
};
#endif