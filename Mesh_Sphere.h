#ifndef __MESH_SPHERE_H
#define __MESH_SPHERE_H

#include <vector>

#include <glm/glm.hpp>

#define MAX_POSITION 1.0f

// Class Mesh_Sphere generates sphere mesh by triangle strip.
class Mesh_Sphere {

public:
	Mesh_Sphere();
	Mesh_Sphere(int density);
	~Mesh_Sphere();

	void InitMesh(int density);

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> uv;
	std::vector<unsigned int> index;

private:
	void GenMeshVertex();
	void GenIndex();
	void GenUV();

	int resolution;
	int row;
	int col;
	float degreePerRow;
	float degreePerCol;
};
#endif