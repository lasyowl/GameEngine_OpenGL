#include "Terrain.h"

#include <GL/glew.h>
#include "Mesh_Terrain.h"

using namespace glm;
using namespace std;

Terrain::Terrain() : quadrant(-1), hasChild(false), parent(nullptr), radius(RADIUS_DEFAULT), numLod(1), lod(0) {
	for (int i = 0; i < 8; i++) {
		neighbor[i] = nullptr;
	}
}

Terrain::~Terrain() {
	glDeleteBuffers(numLod, &ibo[0]);
}

void Terrain::GenChild() {
	int halfChunk = (chunkSize + 1) / 2;
	child.resize(4);
	for (int i = 0; i < 4; i++) {
		child[i].SetMesh(mesh);
		child[i].parent = this;
	}
	child[Q_LD].Initiate(halfChunk, Q_LD, origin + vec2(0, 0), totalSize);
	child[Q_LU].Initiate(halfChunk, Q_LU, origin + vec2(0, halfChunk), totalSize);
	child[Q_RD].Initiate(halfChunk, Q_RD, origin + vec2(halfChunk, 0), totalSize);
	child[Q_RU].Initiate(halfChunk, Q_RU, origin + vec2(halfChunk, halfChunk), totalSize);
}

void Terrain::ConnectNeighbor() {
	if (&child[0] != nullptr) {
		
	}
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
	corner[Q_LD] = vec3(mesh->vertex[index[0][0]]);
	corner[Q_RU] = vec3(mesh->vertex[index[0][index[0].size() - 1]]);
	corner[Q_LU] = vec3(mesh->vertex[index[0][index[0].size() - 1] - chunkSize]);
	corner[Q_RD] = vec3(mesh->vertex[index[0][0] + chunkSize]);
	corner[Q_CENTER] = (corner[Q_LD] + corner[Q_RU]) * 0.5f;

	// Reset y value of corner coords
	for (int i = 0; i < 5; i++) {
		corner[i].y = 0.0f;
	}
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
	// Needs change from here
	cullFlag = DRAW_ALL;
	lod = 0;
	return 0;
	// to here
	validCount = 0;
	int tempLod[4];
	int minLod = 99999999;
	int maxLod = 0;
	int valids[NUM_LOD] = { 0, };
	for (int i = 0; i < 4; i++) {
		float dist = glm::distance(corner[i], cameraPos);
		tempLod[i] = dist / radius;
		
		minLod = min(minLod, tempLod[i]);
		maxLod = max(maxLod, tempLod[i]);

		valids[min(tempLod[i], NUM_LOD - 1)]++;
	}
	int valid = -1;
	for (int i = 0; i < NUM_LOD; i++) {
		if (valids[i] == 4) {
			valid = i;
			break;
		}
	}

	if (valid > -1) {
		if (cameraPos.x > corner[Q_LD].x && cameraPos.x < corner[Q_RD].x && 
			cameraPos.z < corner[Q_LD].z && cameraPos.z > corner[Q_RU].z) {
			if (hasChild == true) {
				cullFlag = DRAW_CHILD;
				for (int i = 0; i < 4; i++) {
					child[i].FrustumTest(cameraPos);
				}
			}
			else {
				lod = minLod;
				cullFlag = DRAW_ALL;
			}
		}
		else {
			cullFlag = DRAW_ALL;
			lod = valid;
		}
	}
	else {
		if (hasChild == true) {
			cullFlag = DRAW_CHILD;
			for (int i = 0; i < 4; i++) {
				child[i].FrustumTest(cameraPos);
			}
		}
		else {
			lod = minLod;
			cullFlag = DRAW_NOTHING;
		}
	}

	return validCount;
}