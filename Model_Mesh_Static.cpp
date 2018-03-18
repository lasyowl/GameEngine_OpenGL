#include <glm/gtc/matrix_transform.hpp>

#include "Model_Mesh_Static.h"
#include "GlobalFunctions.h"

using namespace std;
using namespace glm;

Model_Mesh_Static::Model_Mesh_Static() : materialIndex(-1) {

}

Model_Mesh_Static::~Model_Mesh_Static() {

}

void Model_Mesh_Static::operator = (const aiMesh *mesh) {

	// Stage 1 : Vertex generation
	vertex.resize(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vertex[i].x = mesh->mVertices[i].x;
		vertex[i].y = mesh->mVertices[i].y;
		vertex[i].z = mesh->mVertices[i].z;
	}

	// Stage 2 : Normal generation
	normal.resize(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		normal[i].x = mesh->mNormals[i].x;
		normal[i].y = mesh->mNormals[i].y;
		normal[i].z = mesh->mNormals[i].z;
	}

	// Stage 3 : Texture coordinates generation
	uv.resize(mesh->mNumVertices);
	if (mesh->HasTextureCoords(0)) {
		for (int i = 0; i < mesh->mNumVertices; i++) {
			uv[i].x = mesh->mTextureCoords[0][i].x;
			uv[i].y = mesh->mTextureCoords[0][i].y;
		}
	}

	// Stage 4 : Material index generation
	materialIndex = mesh->mMaterialIndex;

	// Stage 6 : Selection-box generation
	InitSelectionBox(vertex[0]);
	for (int i = 0; i < vertex.size(); i++) {
		UpdateSelectionBox(vertex[i]);
	}
	GenSelectionBoxVertice();
}

void Model_Mesh_Static::InitSelectionBox(const vec3 &point) {
	selectionBox.x_max = point.x;
	selectionBox.x_min = point.x;
	selectionBox.y_max = point.y;
	selectionBox.y_min = point.y;
	selectionBox.z_max = point.z;
	selectionBox.z_min = point.z;
}

void Model_Mesh_Static::UpdateSelectionBox(const vec3 &point) {
	selectionBox.x_max = max(point.x, selectionBox.x_max);
	selectionBox.x_min = min(point.x, selectionBox.x_min);
	selectionBox.y_max = max(point.y, selectionBox.y_max);
	selectionBox.y_min = min(point.y, selectionBox.y_min);
	selectionBox.z_max = max(point.z, selectionBox.z_max);
	selectionBox.z_min = min(point.z, selectionBox.z_min);
}

void Model_Mesh_Static::GenSelectionBoxVertice() {
	vertex_selectionBox.resize(14);
	vertex_selectionBox[0] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[1] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[2] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[3] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[4] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[5] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[6] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[7] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[8] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[9] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[10] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[11] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[12] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[13] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_max);
}