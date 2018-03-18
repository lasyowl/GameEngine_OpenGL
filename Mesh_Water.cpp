#include "Mesh_Water.h"

using namespace glm;

Mesh_Water::Mesh_Water() {

}

void Mesh_Water::SetMesh(const glm::vec2 &xzScale, const glm::vec3 &offset) {
	mesh[0] = vec3(-xzScale.x + offset.x, offset.y, xzScale.y + offset.z);
	mesh[1] = vec3(-xzScale.x + offset.x, offset.y, -xzScale.y + offset.z);
	mesh[2] = vec3(xzScale.x + offset.x, offset.y, xzScale.y + offset.z);
	mesh[3] = vec3(xzScale.x + offset.x, offset.y, -xzScale.y + offset.z);

	uv[0] = vec2(0, 0);
	uv[1] = vec2(0, 10);
	uv[2] = vec2(10, 0);
	uv[3] = vec2(10, 10);
}

Mesh_Water::~Mesh_Water() {

}