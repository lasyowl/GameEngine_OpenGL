#include "Mesh_Quad.h"

using namespace std;
using namespace glm;

Mesh_Quad::Mesh_Quad() {

}

Mesh_Quad::Mesh_Quad(const vec2 &origin, const vec2 &size) {
	SetVariables(size, origin);
}

Mesh_Quad::~Mesh_Quad() {

}

void Mesh_Quad::SetVariables(const vec2 &origin, const vec2 &size) {
	this->size = size;
	this->origin = vec3(origin, 0);
	this->rot = vec3(0, 0, 0);
	GenVertice();
	GenUV();
}

void Mesh_Quad::GenVertice() {
	vertex.resize(4);
	vertex[0] = vec3(-0.5f, -0.5f, 0.0f);
	vertex[1] = vec3(-0.5f, 0.5f, 0.0f);
	vertex[2] = vec3(0.5f, -0.5f, 0.0f);
	vertex[3] = vec3(0.5f, 0.5f, 0.0f);

	for (int i = 0; i < 4; i++) {
		vertex[i].x *= size.x;
		vertex[i].y *= size.y;
		vertex[i].x += 0.5f * size.x + origin.x;
		vertex[i].y += 0.5f * size.y + origin.y;
	}
}

void Mesh_Quad::GenUV() {
	uv.resize(4);
	uv[0] = vec2(0, 0);
	uv[1] = vec2(0, 1);
	uv[2] = vec2(1, 0);
	uv[3] = vec2(1, 1);
}