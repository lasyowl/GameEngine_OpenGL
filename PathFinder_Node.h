#ifndef __PATHFINDER_NODE_H
#define __PATHFINDER_NODE_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <list>

enum PathFinder_Link { PF_LINK_NULLPTR, PF_LINK_EXISTS, PF_LINK_SELF, PF_LINK_SUCCESS };

struct AStar_Value {
	float f;
	float g;
	float h;
};

class PathFinder_Node {
public:
	PathFinder_Node(const glm::vec3 &position, const int &index);
	~PathFinder_Node();

	int LinkNode(PathFinder_Node *node);
	void SetAStar_Value(const float &aStar_g);
	
	int index;
	glm::vec3 position;
	PathFinder_Node *parent;
	std::list<PathFinder_Node*> neighbors;
	std::list<float> length;
	AStar_Value aStar_value;

private:
};
#endif