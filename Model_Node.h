#ifndef __MODEL_NODE_H
#define __MODEL_NODE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assimp/scene.h>

#define MAX_OBJECT_NUM 1000

class Model_Node {
public:
	Model_Node();
	Model_Node(Model_Node *parent);
	~Model_Node();

	void operator = (aiNode *aiNode);

	Model_Node *parent;
	int numChild;
	std::vector<Model_Node> children;
	std::string name;
	int numMesh;
	std::vector<int> meshIndex;
	glm::mat4 transformation;
	glm::mat4 transformation_u[MAX_OBJECT_NUM];

private:
	void SetParent(Model_Node *parent);

};
#endif