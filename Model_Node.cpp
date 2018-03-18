#include "Model_Node.h"
#include "GlobalFunctions.h"

Model_Node::Model_Node() : parent(nullptr), numChild(0), numMesh(0) {

}

Model_Node::Model_Node(Model_Node *parent) : parent(parent), numChild(0), numMesh(0) {

}

Model_Node::~Model_Node() {

}

void Model_Node::operator = (aiNode *aiNode) {
	GlobalFunctions gf;
	transformation = glm::transpose(gf.AiToGlm(aiNode->mTransformation));

	name = aiNode->mName.data;
	numChild = aiNode->mNumChildren;
	children.resize(numChild, this);
	for (int i = 0; i < numChild; i++) {
		children[i] = aiNode->mChildren[i];
	}
	numMesh = aiNode->mNumMeshes;
	meshIndex.resize(numMesh);
	for (int i = 0; i < numMesh; i++) {
		meshIndex[i] = aiNode->mMeshes[i];
	}
}

void Model_Node::SetParent(Model_Node *parent) {
	this->parent = parent;
}