#include "PathFinder_Master.h"
#include "Character.h"

using namespace std;
using namespace glm;

PathFinder_Master::PathFinder_Master() : currentGroupNum(0) {
	currentGroupIter = nodeGroups.begin();
	GenNodeObject();
	BindNodeObject();
}

PathFinder_Master::~PathFinder_Master() {
	
}

void PathFinder_Master::CreateGroup() {
	nodeGroups.push_back(NodeGroup());
	currentGroupIter = nodeGroups.end();
	--currentGroupIter;
	currentGroupNum = nodeGroups.size() - 1;
	currentGroupIter->groupId = currentGroupNum;
	currentGroupIter->nodeIter = currentGroupIter->nodes.begin();
}

void PathFinder_Master::CreateNode(const vec3 &position) {
	currentGroupIter->nodes.push_back(PathFinder_Node(position, currentGroupIter->nodes.size()));
	printf("Total %d nodes\n", currentGroupIter->nodes.size());
}

void PathFinder_Master::LinkNode(PathFinder_Node *start, PathFinder_Node *end) {
	int linkFlag = start->LinkNode(end);
	if (linkFlag == PF_LINK_SUCCESS) {
		GenLineObject(start->position, end->position);
	}
}

PathFinder_Node* PathFinder_Master::FindNearestNode(const vec3 &target, const float &range) {
	list<PathFinder_Node>::iterator iter = currentGroupIter->nodes.begin();
	float minDist = 99999999;
	PathFinder_Node *returnPtr = nullptr;
	for (; iter != currentGroupIter->nodes.end(); ++iter) {
		float temp = distance(target, iter->position);
		if (minDist > temp) {
			minDist = temp;
			returnPtr = &*iter;
		}
	}
	if (minDist > range)
		return nullptr;
	else return returnPtr;
}

void PathFinder_Master::GenLineObject(const vec3 &start, const vec3 &end) {
	currentGroupIter->paths.push_back(PathFinder_Path());
	list<PathFinder_Path>::iterator pathIter = currentGroupIter->paths.end();
	pathIter--;
	pathIter->vertex[0] = start + vec3(0, 1, 0);
	pathIter->vertex[1] = end + vec3(0, 1, 0);
	
	glGenVertexArrays(1, &pathIter->vao);
	glGenBuffers(1, &pathIter->vbo);

	glBindVertexArray(pathIter->vao);

	glBindBuffer(GL_ARRAY_BUFFER, pathIter->vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec3), &pathIter->vertex[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

void PathFinder_Master::GenNodeObject() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &index);
	mesh.InitMesh(5);
}

void PathFinder_Master::BindNodeObject() {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertex.size() * 3 * sizeof(float), &mesh.vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.index.size() * sizeof(GLuint), &mesh.index[0], GL_STATIC_DRAW);
}

vector<PathFinder_Node*> PathFinder_Master::FindPath(Character *character, const vec3 &end) {
	PathFinder_Node *node_start = FindNearestNode(character->GetPos(), 10000);
	PathFinder_Node *node_end = FindNearestNode(end, 10000);
	vector<PathFinder_Node*> retValue;
	
	if (node_start == nullptr || node_end == nullptr) {
		return retValue;
	}

	PathFinder_Node *node_newStart = node_start;
	// F(key) = G(distance from start node) + H(distance to end node)
	node_newStart->aStar_value.g = 0;
	node_newStart->aStar_value.h = distance(node_newStart->position, node_end->position);
	float key = node_newStart->aStar_value.g + node_newStart->aStar_value.h;
	printf("Start node : %d\n", node_newStart->index);
	open_node.Insert(key, node_newStart);

	int count = 0;
	while (1) {
		list<PathFinder_Node*>::iterator nodeIter = open_node.Begin()->neighbors.begin();
		list<float>::iterator lengthIter = open_node.Begin()->length.begin();

		for (; nodeIter != open_node.Begin()->neighbors.end(); ++nodeIter, ++lengthIter) {
			(*nodeIter)->aStar_value.g = node_newStart->aStar_value.g + *lengthIter;
			(*nodeIter)->aStar_value.h = distance((*nodeIter)->position, node_end->position);
			if ((*nodeIter)->aStar_value.h == 0) {
				(*nodeIter)->parent = node_newStart;
				PathFinder_Node *node_trackBack = *nodeIter;
				retValue.resize(2000);
				int retIndex = 0;
				while (node_trackBack != node_start) {
					retValue[retIndex] = node_trackBack;
					retIndex++;
					node_trackBack = node_trackBack->parent;
				}
				retValue[retIndex] = node_start;
				retIndex++;
				retValue.resize(retIndex);
				reverse(retValue.begin(), retValue.end());

				open_node.Clear();
				closed_node.clear();
				return retValue;
			}
			key = (*nodeIter)->aStar_value.g + (*nodeIter)->aStar_value.h;
			list<PathFinder_Node*>::iterator closedIter = closed_node.begin();
			if (find(closedIter, closed_node.end(), *nodeIter) == closed_node.end()) {
				open_node.Insert(key, *nodeIter);
				(*nodeIter)->parent = node_newStart;
			}
		}
		PathFinder_Node *node_closed = open_node.Delete();
		closed_node.push_back(node_closed);

		node_newStart = open_node.Begin();
		count++;
		if (count > 100) {
			printf("Too much iteration\n");
			break;
		}
	}
	return retValue;
}