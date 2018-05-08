#include "PathFinder_Node.h"

#include <algorithm>

using namespace glm;
using namespace std;

PathFinder_Node::PathFinder_Node(const vec3 &position, const int &index) : parent(nullptr) {
	this->position = position;
	this->index = index;
}

PathFinder_Node::~PathFinder_Node() {

}

int PathFinder_Node::LinkNode(PathFinder_Node *node) {
	if (node == nullptr)
		return PF_LINK_NULLPTR;

	list<PathFinder_Node*>::iterator iter = find(neighbors.begin(), neighbors.end(), node);
	// 1. Link exists
	if (iter != neighbors.end()) {
		printf("Link already exists\n");
		return PF_LINK_EXISTS;
	}
	// 2. Reject self-link
	else if (this == node) {
		printf("Self-link unavailable\n");
		return PF_LINK_SELF;
	}
	// 3. Link success
	else {
		float length = distance(position, node->position);
		neighbors.push_back(node);
		this->length.push_back(length);
		node->neighbors.push_back(this);
		node->length.push_back(length);
		printf("Connected node %f and node %f\n", this->position.x, (*(--neighbors.end()))->position.x);
		return PF_LINK_SUCCESS;
	}
}