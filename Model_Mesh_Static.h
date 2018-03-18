#ifndef __MODEL_MESH_STATIC_H
#define __MODEL_MESH_STATIC_H

#include <vector>
#include <map>
#include <string>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include "SelectionBox.h"

#define MAX_WEIGHT_BONE_NUMBER 4

class Model_Mesh_Static {
public:
	Model_Mesh_Static();
	~Model_Mesh_Static();

	void InitSelectionBox(const glm::vec3 &point);
	void UpdateSelectionBox(const glm::vec3 &point);
	void GenSelectionBoxVertice();

	void operator = (const aiMesh *mesh);

	std::string name;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> uv;

	SelectionBox selectionBox;
	std::vector<glm::vec3> vertex_selectionBox;

	int materialIndex;

private:

};
#endif