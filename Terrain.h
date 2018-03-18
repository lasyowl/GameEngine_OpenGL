#ifndef __TERRAIN_H
#define __TERRAIN_H

#define MIN_CHUNK_SIZE 64
#define NUM_LOD 4

#include <vector>
#include <glm/glm.hpp>

typedef unsigned int GLuint;

class Mesh_Terrain;

enum Quadrant { LD, LU, RD, RU, CENTER };
enum DrawState { DRAW_ALL, DRAW_NOTHING, DRAW_CHILD };

class Terrain {
public:
	Terrain();
	~Terrain();

	void Initiate(const int &chunkSize, const int &quadrant, const glm::vec2 &origin, const int &totalSize);
	void GenObject();
	void SetMesh(Mesh_Terrain *mesh);
	void SetRadius(const float &radius);
	GLuint GetIbo(const int &lod);
	int FrustumTest(const glm::vec3 &cameraPos);

	std::vector<Terrain> child;
	std::vector<int> numIndex;
	bool hasChild;
	int inbound;
	int validCount;
	std::vector<std::vector<int>> index;
	glm::vec3 corner[5];
	
private:
	void GenChild();
	void GenIndex();

	Mesh_Terrain *mesh;
	int totalSize;
	int chunkSize;
	int quadrant;
	int numLod;
	float radius;
	glm::vec2 origin;
	std::vector<GLuint> ibo;
};
#endif