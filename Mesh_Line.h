#ifndef __MESH_LINE_H
#define __MESH_LINE_H

#include <vector>

#include <glm/glm.hpp>

class Mesh_Line {

public:
	Mesh_Line(const glm::vec3 &start, const glm::vec3 &end);
	~Mesh_Line();

	std::vector<glm::vec3> vertex;

private:

};
#endif