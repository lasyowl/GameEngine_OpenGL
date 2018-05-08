#ifndef __PATHFINDER_MASTER_H
#define __PATHFINDER_MASTER_H

#include "PathFinder_Node.h"
#include "P_Queue_Master.cpp"
#include "Mesh_Sphere.h"

class Character;

struct PathFinder_Path {
	glm::vec3 vertex[2];
	GLuint vao;
	GLuint vbo;
};

struct NodeGroup {
	int groupId;
	std::list<PathFinder_Node> nodes;
	std::list<PathFinder_Node>::iterator nodeIter;
	std::list<PathFinder_Path> paths;
};

class PathFinder_Master {
public:
	PathFinder_Master();
	~PathFinder_Master();

	void CreateGroup();
	void CreateNode(const glm::vec3 &position);
	void LinkNode(PathFinder_Node *start, PathFinder_Node *end);
	PathFinder_Node* FindNearestNode(const glm::vec3 &target, const float &range = 3.0f);
	std::vector<PathFinder_Node*> FindPath(Character *character, const glm::vec3 &end);

	std::list<NodeGroup> nodeGroups;
	int currentGroupNum;
	std::list<NodeGroup>::iterator currentGroupIter;
	Mesh_Sphere mesh;
	GLuint vao;

private:
	void GenLineObject(const glm::vec3 &start, const glm::vec3 &end);
	void GenNodeObject();
	void BindNodeObject();

	P_Queue_Master<float, PathFinder_Node*> open_node;
	std::list<PathFinder_Node*> closed_node;
	GLuint vbo_vertex;
	GLuint index;
};
#endif