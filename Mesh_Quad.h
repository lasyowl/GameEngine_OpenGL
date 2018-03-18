#ifndef __MESH_QUAD_H
#define __MESH_QUAD_H

#include <vector>
#include <glm/glm.hpp>

class Mesh_Quad {
public:
	Mesh_Quad();
	Mesh_Quad(const glm::vec2 &origin, const glm::vec2 &size);
	~Mesh_Quad();

	void SetVariables(const glm::vec2 &origin, const glm::vec2 &size);

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec2> uv;
	glm::vec2 size;
	glm::vec3 origin;
	glm::vec3 rot;

private:
	void GenVertice();
	void GenUV();

};
#endif;