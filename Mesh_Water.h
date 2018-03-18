#ifndef __MESH_WATER_H
#define __MESH_WATER_H

#include <glm/glm.hpp>

class Mesh_Water {
public:
	Mesh_Water();
	~Mesh_Water();

	void SetMesh(const glm::vec2 &xzScale, const glm::vec3 &offset);

	glm::vec3 mesh[4];
	glm::vec2 uv[4];

private:
};
#endif