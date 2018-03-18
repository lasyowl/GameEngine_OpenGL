#include "ModelTransform_Loader.h"

using namespace std;
using namespace glm;

ModelTransform_Loader::ModelTransform_Loader() : numObjects(0), numAnimated(0), numStatic(0) {

}

ModelTransform_Loader::~ModelTransform_Loader() {

}

void ModelTransform_Loader::ReadFile(string fileName) {
	FILE *file;
	fopen_s(&file, fileName.data(), "rb");
	char buffer[200];
	char buffer_s[100];

	do {
		fgets(buffer, 200, file);
		if(buffer[0] == '#')
			headerInfo_a += buffer;
	}
	while (buffer[0] == '#');

	sscanf_s(buffer, "%d", &numAnimated);
	animatedIndex.resize(numAnimated);
	for (int i = 0; i < numAnimated; i++) {
		fgets(buffer, 200, file);
		if (buffer[0] == '#')
			break;
		sscanf_s(buffer, "%d / %s", &animatedIndex[i].index, buffer_s);
		animatedIndex[i].name = buffer_s;
		headerInfo_b += buffer;
	}

	do {
		fgets(buffer, 200, file);
		if (buffer[0] == '#')
			headerInfo_b += buffer;
	} while (buffer[0] == '#');

	sscanf_s(buffer, "%d", &numStatic);
	staticIndex.resize(numStatic);
	for (int i = 0; i < numStatic; i++) {
		fgets(buffer, 200, file);
		headerInfo_c += buffer;
		if (buffer[0] == '#')
			break;
		sscanf_s(buffer, "%d / %s", &staticIndex[i].index, buffer_s);
		staticIndex[i].name = buffer_s;
	}

	do {
		fgets(buffer, 200, file);
		if(buffer[0] == '#')
			headerInfo_c += buffer;
	} while (buffer[0] == '#');

	sscanf_s(buffer, "%d", &numObjects);
	objInfo.resize(numObjects);

	for (int i = 0; i < numObjects; i++) {
		fgets(buffer, 200, file);
		sscanf_s(buffer, "%d / %f %f %f / %f %f %f / %f %f %f / %f / %d", &objInfo[i].modelId, &objInfo[i].pos.x, &objInfo[i].pos.y, &objInfo[i].pos.z,
			&objInfo[i].dir.x, &objInfo[i].dir.y, &objInfo[i].dir.z, &objInfo[i].preRot.x, &objInfo[i].preRot.y, &objInfo[i].preRot.z,
			&objInfo[i].scale, &objInfo[i].tag);
	}

	fclose(file);
}

void ModelTransform_Loader::BeginSaving(std::string fileName, const int &numObjects) {
	fopen_s(&file_save, fileName.data(), "wb");
	char buffer[200];
	fprintf_s(file_save, "%s%d\n%s%d\n%s", headerInfo_a.data(), numAnimated, headerInfo_b.data(), numStatic, headerInfo_c.data());
	fprintf_s(file_save, "%d\n", numObjects);
}

void ModelTransform_Loader::SaveData(const ObjectInfo &objInfo) {
	fprintf_s(file_save, "%d / %f %f %f / %f %f %f / %f %f %f / %f / %d\n", objInfo.modelId, objInfo.pos.x, objInfo.pos.y, objInfo.pos.z, objInfo.dir.x, objInfo.dir.y, objInfo.dir.z,
		objInfo.preRot.x, objInfo.preRot.y, objInfo.preRot.z, objInfo.scale, objInfo.tag);
}

void ModelTransform_Loader::FinishSaving() {
	fclose(file_save);
}