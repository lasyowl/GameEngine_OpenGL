#include <glm/gtc/matrix_transform.hpp>

#include "Model_Mesh_Animated.h"
#include "GlobalFunctions.h"
#include "Model_Node.h"

using namespace std;
using namespace glm;

Model_Mesh_Animated::Model_Mesh_Animated() : numBoneMatrix(0), materialIndex(-1) {
	for (int i = 0; i < 8; i++) {
		selectionBox.vertices[i] = vec3(0.0f);
	}
}

Model_Mesh_Animated::~Model_Mesh_Animated() {

}

void Model_Mesh_Animated::operator = (const aiMesh *mesh) {
	GlobalFunctions gf;

	// Stage 1 : Vertex generation
	vertex_model.resize(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vertex_model[i].x = mesh->mVertices[i].x;
		vertex_model[i].y = mesh->mVertices[i].y;
		vertex_model[i].z = mesh->mVertices[i].z;
	}

	// Stage 2 : Normal generation
	normal_model.resize(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		normal_model[i].x = mesh->mNormals[i].x;
		normal_model[i].y = mesh->mNormals[i].y;
		normal_model[i].z = mesh->mNormals[i].z;
	}

	// Stage 3 : Texture coordinates generation
	uv_model.resize(mesh->mNumVertices);
	if (mesh->HasTextureCoords(0)) {
		for (int i = 0; i < mesh->mNumVertices; i++) {
			uv_model[i].x = mesh->mTextureCoords[0][i].x;
			uv_model[i].y = mesh->mTextureCoords[0][i].y;
		}
	}
	
	// Stage 4 : Weight, boneID and bone matrix data generation
	weight.resize(MAX_WEIGHT_BONE_NUMBER * mesh->mNumVertices, 0.0f);
	boneID.resize(MAX_WEIGHT_BONE_NUMBER * mesh->mNumVertices, 0);
	boneMatrix.resize(MAX_WEIGHT_BONE_NUMBER * mesh->mNumBones);
	numBoneMatrix = boneMatrix.size();

	boneMatrix_u.resize(MAX_OBJECT_NUM);
	for (int i = 0; i < MAX_OBJECT_NUM; i++) {
		boneMatrix_u[i].resize(MAX_WEIGHT_BONE_NUMBER * mesh->mNumBones);
	}

	for (int i = 0; i < mesh->mNumBones; i++) {
		aiBone *bone_temp = mesh->mBones[i];
		boneMap[bone_temp->mName.data] = i;
		boneMatrix[i] = glm::transpose(gf.AiToGlm(bone_temp->mOffsetMatrix));
		for (int j = 0; j < bone_temp->mNumWeights; j++) {
			aiVertexWeight *weight_temp = &bone_temp->mWeights[j];
			for (int k = 0; k < MAX_WEIGHT_BONE_NUMBER; k++) {
				if (weight[MAX_WEIGHT_BONE_NUMBER * weight_temp->mVertexId + k] == 0) {
					weight[MAX_WEIGHT_BONE_NUMBER * weight_temp->mVertexId + k] = weight_temp->mWeight;
					boneID[MAX_WEIGHT_BONE_NUMBER * weight_temp->mVertexId + k] = i;
					break;
				}
			}
		}
	}

	// Stage 5 : Material index generation
	materialIndex = mesh->mMaterialIndex;

	// Stage 6 : Selection-box generation
	InitSelectionBox(vertex_model[0]);
	for (int i = 0; i < vertex_model.size(); i++) {
		UpdateSelectionBox(vertex_model[i]);
	}
	GenSelectionBoxVertice();
}

void Model_Mesh_Animated::InitSelectionBox(const vec3 &point) {
	selectionBox.x_max = point.x;
	selectionBox.x_min = point.x;
	selectionBox.y_max = point.y;
	selectionBox.y_min = point.y;
	selectionBox.z_max = point.z;
	selectionBox.z_min = point.z;
}

void Model_Mesh_Animated::UpdateSelectionBox(const vec3 &point) {
	selectionBox.x_max = max(point.x, selectionBox.x_max);
	selectionBox.x_min = min(point.x, selectionBox.x_min);
	selectionBox.y_max = max(point.y, selectionBox.y_max);
	selectionBox.y_min = min(point.y, selectionBox.y_min);
	selectionBox.z_max = max(point.z, selectionBox.z_max);
	selectionBox.z_min = min(point.z, selectionBox.z_min);
}

void Model_Mesh_Animated::GenSelectionBoxVertice() {
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