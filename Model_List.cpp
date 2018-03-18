#include <stdio.h>

#include "Model_List.h"

using namespace std;

Model_List::Model_List() {

}
Model_List::~Model_List() {

}

void Model_List::ReadFile(string filePath) {
	FILE *file;
	fopen_s(&file, filePath.data(), "rb");
	char buf[1000];
	char bufA[1000], bufB[1000];
	bool addAnim = false;
	fgets(buf, 1000, file);
	fgets(buf, 1000, file);
	fgets(buf, 1000, file);
	fgets(buf, 1000, file);
	unsigned int numModels;
	sscanf_s(buf, "%u", &numModels);
	modelList.resize(numModels);

	for (int i = 0; i < numModels; i++) {
		fgets(buf, 1000, file);
		int pos = 0;
		for (int j = 0; j < 1000; j++) {
			bufA[j] = buf[j];
			if (bufA[j] == '@') {
				bufA[j] = '\0';
				pos = j + 1;
				break;
			}
		}
		for (int j = 0; j < 1000; j++) {
			bufB[j] = buf[pos + j];
			if (bufB[j] == '@') {
				bufB[j] = '\0';
				pos = pos + j + 1;
				addAnim = true;
				break;
			}
			else if (bufB[j] == '$') {
				bufB[j] = '\0';
				break;
			}
		}
		modelList[i].filePath = bufA;
		modelList[i].fileName = bufB;
		modelList[i].fullPath = modelList[i].filePath + modelList[i].fileName;
		printf("%s\n", modelList[i].fullPath.data());
		
		modelList[i].additionalAnims = 0;
		if (addAnim == true) {
			int index = 0;
			while (1) {
				bufA[index] = buf[pos];
				pos++;
				if (bufA[index] == '@') {
					bufA[index] = '\0';
					modelList[i].animList.push_back(bufA);
					modelList[i].additionalAnims++;
					index = 0;
					continue;
				}
				else if (bufA[index] == '$') {
					bufA[index] = '\0';
					modelList[i].animList.push_back(bufA);
					modelList[i].additionalAnims++;
					break;
				}
				index++;
			}
		}
	}
	fclose(file);
}