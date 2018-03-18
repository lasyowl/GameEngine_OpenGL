#ifndef __MODELCOORD_LOADER_H
#define __MODELCOORD_LOADER_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Object_Info.h"

struct ModelIndexDatabase {
	int index;
	std::string name;
};

class ModelTransform_Loader {

public:
	ModelTransform_Loader();
	~ModelTransform_Loader();

	void ReadFile(std::string fileName);
	void BeginSaving(std::string fileName, const int &numObjects);
	void SaveData(const ObjectInfo &objInfo);
	void FinishSaving();

	std::vector<ObjectInfo> objInfo;
	int numObjects;
	std::vector<ModelIndexDatabase> animatedIndex, staticIndex;
	int numAnimated, numStatic;
	std::string headerInfo_a;
	std::string headerInfo_b;
	std::string headerInfo_c;

private:
	FILE *file_save;
};
#endif