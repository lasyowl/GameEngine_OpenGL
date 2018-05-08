#include "Mesh_Sphere.h"

using namespace glm;

Mesh_Sphere::Mesh_Sphere() {
}

Mesh_Sphere::Mesh_Sphere(int resolution) {
	InitMesh(resolution);
}

Mesh_Sphere::~Mesh_Sphere() {

}

void Mesh_Sphere::InitMesh(int resolution) {
	this->resolution = resolution;
	col = resolution;
	row = resolution / 2 + 1;
	vertex.resize(row * col);
	normal.resize(row * col);
	degreePerRow = 180.0f / (row - 1);
	degreePerCol = 360.0f / (col - 1);

	GenMeshVertex();
	GenIndex();
	GenUV();
}

// Vertex and normal generation.
// For sphere, normal is same as normalized vertex.
void Mesh_Sphere::GenMeshVertex() {
	for (int i = 0; i < row; i++) {
		float y = MAX_POSITION * cos(radians(degreePerRow * i));
		for (int j = 0; j < col; j++) {
			float length_projection = sqrt(1 - pow(y, 2));
			float x = length_projection * cos(radians(degreePerCol * j));
			float z = -length_projection * sin(radians(degreePerCol * j));
			vertex[i * col + j] = vec3(x, y, z);
			normal[i * col + j] = normalize(vec3(x, y, z));
		}
	}
}

void Mesh_Sphere::GenIndex() {
	index.resize(2 * row * col - 2 * col + 2 * (row - 2));
	for (int i = 0; i < row - 1; i++) {
		for (int j = 0; j < col * 2; j++) {
			index[(2 * col + 2) * i + j] = j / 2 + col * (i + j % 2);
		}
		if (i < col - 2) {
			index[(2 * col + 2) * i + col * 2] = index[(2 * col + 2) * i + col * 2 - 1];
			index[(2 * col + 2) * i + col * 2 + 1] = index[(2 * col + 2) * i + col * 2] - col + 1;
		}
	}
}

void Mesh_Sphere::GenUV() {
	vec2 textureScale = vec2(1.0f / (col - 1), 1.0f / (row - 1));
	//float textureScale = 1.0f / radians(360.0f);
	uv.resize(row * col);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			uv[i * col + j] = vec2(1 - j * textureScale.x, 1 - i * textureScale.y);
		}
	}
}