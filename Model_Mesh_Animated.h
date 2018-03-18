#ifndef __MODEL_MESH_ANIMATED_H
#define __MODEL_MESH_ANIMATED_H

#include <vector>
#include <map>
#include <string>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include "SelectionBox.h"

#define MAX_WEIGHT_BONE_NUMBER 4

class Model_Mesh_Animated {
public:
	Model_Mesh_Animated();
	~Model_Mesh_Animated();

	void InitSelectionBox(const glm::vec3 &point);
	void UpdateSelectionBox(const glm::vec3 &point);
	void GenSelectionBoxVertice();

	void operator = (const aiMesh *mesh);

	std::string name;
	std::vector<glm::vec3> vertex_model;
	std::vector<glm::vec3> normal_model;
	std::vector<glm::vec2> uv_model;
	std::vector<float> weight;
	std::vector<int> boneID;
	std::vector<glm::mat4> boneMatrix;
	std::vector<std::vector<glm::mat4>> boneMatrix_u;
	std::map<std::string, int> boneMap;

	SelectionBox selectionBox;
	std::vector<glm::vec3> vertex_selectionBox;

	int numBoneMatrix;
	int materialIndex;

private:

};
#endif