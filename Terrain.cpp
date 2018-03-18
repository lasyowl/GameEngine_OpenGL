#include "Terrain.h"

#include <GL/glew.h>
#include "Mesh_Terrain.h"

using namespace glm;
using namespace std;

Terrain::Terrain() : quadrant(-1), hasChild(false), radius(200) {

}

Terrain::~Terrain() {
	glDeleteBuffers(numLod, &ibo[0]);
}

void Terrain::GenChild() {
	int halfChunk = (chunkSize + 1) / 2;
	child.resize(4);
	for (int i = 0; i < 4; i++) {
		child[i].SetMesh(mesh);
	}
	child[LD].Initiate(halfChunk, LD, origin + vec2(0, 0), totalSize);
	child[LU].Initiate(halfChunk, LU, origin + vec2(0, halfChunk), totalSize);
	child[RD].Initiate(halfChunk, RD, origin + vec2(halfChunk, 0), totalSize);
	child[RU].Initiate(halfChunk, RU, origin + vec2(halfChunk, halfChunk), totalSize);
}

void Terrain::SetMesh(Mesh_Terrain *mesh) {
	this->mesh = mesh;
	if (hasChild == true) {
		for (int i = 0; i < 4; i++) {
			child[i].SetMesh(mesh);
		}
	}
}

void Terrain::SetRadius(const float &radius) {
	this->radius = radius;
	if(hasChild)
		for (int i = 0; i < 4; i++) {
			child[i].SetRadius(radius);
		}
}

void Terrain::Initiate(const int &chunkSize, const int &quadrant, const vec2 &origin, const int &totalSize) {
	this->chunkSize = chunkSize;
	this->origin = origin;
	this->totalSize = totalSize;
	if(chunkSize < totalSize)
		GenIndex();

	if (chunkSize > MIN_CHUNK_SIZE) {
		GenChild();
		hasChild = true;
	}
}

GLuint Terrain::GetIbo(const int &lod) {
	return ibo[min(lod, this->numLod - 1)];
}

void Terrain::GenIndex() {
	int x = origin.x;
	int y = -origin.y;
	int end_x = x + chunkSize;
	int end_y = y + chunkSize;
	int index_dy = totalSize + 1;
	int chunkPlus = chunkSize + 1;
	int startIndex = origin.y * index_dy + origin.x;
	int numVertex = pow(chunkSize + 1, 2);

	numLod = NUM_LOD;
	index.resize(NUM_LOD);
	for (int i = 0; i < NUM_LOD; i++) {
		int lod_pow = pow(2, i) - 1;

		int chunkSize_lod = chunkSize / (lod_pow + 1);
		int chunkPlus_lod = chunkSize_lod + 1;
		int numIndex = chunkPlus_lod * 2 * chunkSize_lod + 2 * (chunkSize_lod - 1);

		if (chunkSize_lod < MIN_CHUNK_SIZE) {
			numLod = i;
			break;
		}

		vector<int> temp;
		temp.resize(numIndex);
		temp[0] = startIndex;
		temp[1] = startIndex + (1 + lod_pow) * index_dy;
		for (int i = 2, n = 1; i < numIndex; i += 2, n += lod_pow + 1) {
			if (n == 0) {
				temp[i] = temp[i - 2] - chunkSize;
			}
			else if (n < chunkPlus) {
				temp[i] = temp[i - 2] + 1 + lod_pow;
			}
			else if (n >= chunkPlus) {
				temp[i] = temp[i - 2] + (1 + lod_pow) * index_dy;
				n = -1 - lod_pow;
			}
		}

		for (int i = 3, n = 1; i < numIndex; i += 2, n += lod_pow + 1) {
			if (n < chunkPlus) {
				temp[i] = temp[i - 1] + (1 + lod_pow) * index_dy;
			}
			else if (n >= chunkPlus) {
				temp[i] = temp[i - 1] - chunkSize;
				n = -1 - lod_pow;
			}
		}
		
		index[i] = temp;
	}
	index.resize(numLod);

	numIndex.resize(numLod);
	for (int i = 0; i < numLod; i++) {
		numIndex[i] = index[i].size();
	}
	corner[LD] = vec3(mesh->vertex[index[0][0]]);
	corner[RU] = vec3(mesh->vertex[index[0][index[0].size() - 1]]);
	corner[LU] = vec3(mesh->vertex[index[0][index[0].size() - 1] - chunkSize]);
	corner[RD] = vec3(mesh->vertex[index[0][0] + chunkSize]);
	corner[CENTER] = (corner[LD] + corner[RU]) * 0.5f;
}

void Terrain::GenObject() {
	ibo.resize(numLod);
	glGenBuffers(numLod, &ibo[0]);
	for (int i = 0; i < index.size(); i++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index[i].size() * sizeof(int), &index[i][0], GL_STATIC_DRAW);
	}
	if (hasChild) {
		for (int j = 0; j < 4; j++) {
			child[j].GenObject();
		}
	}
}

int Terrain::FrustumTest(const vec3 &cameraPos) {
	validCount = 0;
	float minDist = 99999999;
	for (int i = 0; i < 4; i++) {
		float dist = glm::distance(corner[i], cameraPos);
		if (dist > radius) {
			// outside of frustum
			minDist = min(dist, minDist);
		}
		else {
			// inside of frustum
			validCount++;
		}
	}

	if (validCount == 0) {
		if (radius < chunkSize / 2) {
			if (hasChild == true) {
				inbound = DRAW_CHILD;
				for (int i = 0; i < 4; i++) {
					child[i].FrustumTest(cameraPos);
				}
			}
			else {
				inbound = DRAW_ALL;
			}
		}
		else {
			inbound = DRAW_NOTHING;
		}
	}
	else {
		if (validCount == 4) {
			inbound = DRAW_ALL;
		}
		else if (hasChild == false) {
			inbound = DRAW_ALL;
		}
		else if (hasChild == true) {
			inbound = DRAW_CHILD;
			for (int i = 0; i < 4; i++) {
				child[i].FrustumTest(cameraPos);
			}
		}
	}

	return validCount;
}