#ifndef __TERRAIN_H
#define __TERRAIN_H

#define MIN_CHUNK_SIZE 128
#define NUM_LOD 4
#define RADIUS_DEFAULT 200.0f

#include <vector>
#include <glm/glm.hpp>

typedef unsigned int GLuint;

class Mesh_Terrain;

enum Quadrant { Q_LD, Q_LU, Q_RD, Q_RU, Q_CENTER };
enum DrawState { DRAW_ALL, DRAW_NOTHING, DRAW_CHILD };
enum Neighbor { LL, LU, UU, RU, RR, RD, DD, LD };

class Terrain {
public:
	Terrain();
	~Terrain();

	void Initiate(const int &chunkSize, const int &quadrant, const glm::vec2 &origin, const int &totalSize);
	void GenObject();
	void SetMesh(Mesh_Terrain *mesh);
	void SetRadius(const float &radius);
	void ConnectNeighbor();
	GLuint GetIbo(const int &lod);
	int FrustumTest(const glm::vec3 &cameraPos);

	Terrain *parent;
	Terrain *neighbor[8];
	std::vector<Terrain> child;
	std::vector<int> numIndex;
	bool hasChild;
	int cullFlag;
	int validCount;
	std::vector<std::vector<int>> index;
	glm::vec3 corner[5];
	int lod;
	
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