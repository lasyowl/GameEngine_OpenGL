#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <FreeImage/FreeImage.h>

#include "Model_Scene_Animated.h"
#include "GlobalFunctions.h"

using namespace std;
using namespace glm;

#define INV_ONE_MS 0.001f

Model_Scene_Animated::Model_Scene_Animated() : numAnims(0), animState(STATE_IDLE) {
	for (int i = 0; i < MAX_OBJECT_NUM; i++) {
		currentAnimTime[i] = 0;
		recentAnim[i] = nullptr;
	}
}

Model_Scene_Animated::~Model_Scene_Animated() {
	glDeleteVertexArrays(vao_model.size(), &vao_model[0]);
	glDeleteBuffers(vbo_model_vertex.size(), &vbo_model_vertex[0]);
	glDeleteBuffers(vbo_model_normal.size(), &vbo_model_normal[0]);
	glDeleteBuffers(vbo_model_uv.size(), &vbo_model_uv[0]);
	glDeleteBuffers(vbo_model_weight.size(), &vbo_model_weight[0]);
	glDeleteBuffers(vbo_model_boneID.size(), &vbo_model_boneID[0]);

	glDeleteVertexArrays(1, &vao_selectionBox[0]);
	glDeleteBuffers(1, &vbo_selectionBox_vertex[0]);
}

int Model_Scene_Animated::GetNumVao() {
	return vao_model.size();
}

int Model_Scene_Animated::GetNumVertex(int index) {
	return numVertex[index];
}

void Model_Scene_Animated::BindVao_Model(int index) {
	glBindVertexArray(vao_model[index]);
}

void Model_Scene_Animated::BindVao_SelectionBox(int index) {
	glBindVertexArray(vao_selectionBox[index]);
}

void Model_Scene_Animated::LoadTexture(const char *filePath) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	GLvoid *data;

	if (*filePath == NULL) {
		return;
	}
	tx_file_format = FreeImage_GetFileType(filePath, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filePath);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32 = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32);
	height = FreeImage_GetHeight(tx_pixmap_32);
	data = FreeImage_GetBits(tx_pixmap_32);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
}

void Model_Scene_Animated::LoadAdditionalAnims(FileInfo &fileInfo) {
	int newStart = numAnims - fileInfo.additionalAnims;
	list<string>::iterator iter = fileInfo.animList.begin();
	char buffer[100];
	while (iter != fileInfo.animList.end()) {
		Assimp::Importer animImporter;
		string file = fileInfo.filePath + *iter;
		animImporter.ReadFile(file, aiProcess_Triangulate | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
		const aiScene *anim_temp = animImporter.GetOrphanedScene();
		
		animations[newStart] = anim_temp->mAnimations[0];
		//printf("%s\n", anim_temp->mAnimations[0]->mName);
		for (int i = 0; i < iter->length(); i++) {
			buffer[i] = (*iter)[i];
			if (buffer[i] == '.') {
				buffer[i] = '\0';
				break;
			}
		}
		animations[newStart].name = buffer;
		newStart++;
		iter++;
	}
}

void Model_Scene_Animated::LoadScene(FileInfo &fileInfo) {
	Assimp::Importer importer;

	importer.ReadFile(fileInfo.fullPath.data(), aiProcess_Triangulate | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
	const aiScene *scene = importer.GetOrphanedScene();

	// Stage 1 : Mesh generation
	mesh.resize(scene->mNumMeshes);
	numVertex.resize(mesh.size());
	for (int i = 0; i < scene->mNumMeshes; i++) {
		mesh[i] = scene->mMeshes[i];
		numVertex[i] = mesh[i].vertex_model.size();
	}

	// Stage 2 : Animation generation
	numAnims = scene->mNumAnimations + fileInfo.additionalAnims;
	animations.resize(numAnims);
	for (int i = 0; i < numAnims - fileInfo.additionalAnims; i++) {
		animations[i] = scene->mAnimations[i];
	}
	LoadAdditionalAnims(fileInfo);
	

	// Stage 3 : Node generation
	rootNode = scene->mRootNode;
	GlobalFunctions gf;
	GlobalInvMatrix = glm::inverse(rootNode.transformation);
	// And post generation
	//NodePostProcess(&rootNode);

	// Stage 4 : Material generation
	numMaterials = scene->mNumMaterials;
	materials.resize(numMaterials);
	for (int i = 0; i < numMaterials; i++) {
		aiString buffer;
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &buffer) == AI_SUCCESS) {
			materials[i].texture_diffuse = buffer.data;
			materials[i].useTexture = true;

			// **** Temporary patch for texture load ****
			if (materials[i].texture_diffuse[0] == '.')
				materials[i].texture_diffuse = materials[i].texture_diffuse.substr(3, materials[i].texture_diffuse.length() - 3);
			
			GLuint tbo_temp;
			glGenTextures(1, &tbo_temp);
			glActiveTexture(GL_TEXTURE0 + tbo_temp);
			glBindTexture(GL_TEXTURE_2D, tbo_temp);
			LoadTexture((fileInfo.filePath + materials[i].texture_diffuse).data());
			glGenerateMipmap(GL_TEXTURE_2D);
			materials[i].tbo_diffuse = tbo_temp;

			printf("%s\n", (fileInfo.filePath + materials[i].texture_diffuse).data());
		}
		aiColor4D ambient, diffuse, specular;
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_AMBIENT, &ambient);
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &diffuse);
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_SPECULAR, &specular);
		materials[i].CopyColor(ambient, COLOR_AMBIENT);
		materials[i].CopyColor(diffuse, COLOR_DIFFUSE);
		materials[i].CopyColor(specular, COLOR_SPECULAR);
	}
	
	// Stage 5 : Selection-box generation
	for (int i = 0; i < scene->mNumMeshes; i++) {
		selectionBox.x_max = std::max(selectionBox.x_max, mesh[i].selectionBox.x_max);
		selectionBox.x_min = std::min(selectionBox.x_min, mesh[i].selectionBox.x_min);
		selectionBox.y_max = std::max(selectionBox.y_max, mesh[i].selectionBox.y_max);
		selectionBox.y_min = std::min(selectionBox.y_min, mesh[i].selectionBox.y_min);
		selectionBox.z_max = std::max(selectionBox.z_max, mesh[i].selectionBox.z_max);
		selectionBox.z_min = std::min(selectionBox.z_min, mesh[i].selectionBox.z_min);
	}
	GenSelectionBoxVertice();

	GenObjects();
	BindObjectData();

	numMeshes = mesh.size();
	importer.FreeScene();
}

void Model_Scene_Animated::GenSelectionBoxVertice() {
	vertex_selectionBox.resize(16);
	vertex_selectionBox[0] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[1] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[2] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[3] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[4] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_max);
	vertex_selectionBox[5] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[6] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[7] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[8] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[9] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[10] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[11] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[12] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_max);
	vertex_selectionBox[13] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_min);
	vertex_selectionBox[14] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_min);
	vertex_selectionBox[15] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_max);

	selectionBox.vertices[0] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_max);
	selectionBox.vertices[1] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_max);
	selectionBox.vertices[2] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_max);
	selectionBox.vertices[3] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_max);
	selectionBox.vertices[4] = vec3(selectionBox.x_min, selectionBox.y_min, selectionBox.z_min);
	selectionBox.vertices[5] = vec3(selectionBox.x_min, selectionBox.y_max, selectionBox.z_min);
	selectionBox.vertices[6] = vec3(selectionBox.x_max, selectionBox.y_max, selectionBox.z_min);
	selectionBox.vertices[7] = vec3(selectionBox.x_max, selectionBox.y_min, selectionBox.z_min);
}

void Model_Scene_Animated::SetWorldPosition(glm::vec3 &worldPosition) {
	this->worldPosition = worldPosition;
}

void Model_Scene_Animated::NodePostProcess(Model_Node *node) {
	for (int i = 0; i < node->numChild; i++) {
		NodePostProcess(&node->children[i]);
	}
}

void Model_Scene_Animated::PlayAnim(const string &animName, const int &objectNum, const int &playCount) {
	recentAnim[objectNum] = currentAnim[objectNum];
	currentAnim[objectNum] = FindAnim(animName);
	this->playCount[objectNum] = playCount;
	animState = STATE_SWITCHING;
}

void Model_Scene_Animated::MaintainAnim(const int &objectNum) {
	this->playCount[objectNum] = 100;
}

void Model_Scene_Animated::Reset(const int &objectNum) {
	currentAnimTime[this->currentObjectNum] = 0;
	interpolateFactor[objectNum] = 0;
}

void Model_Scene_Animated::UpdateBoneMatrix(int timeElapsed, mat4 &WorldMatrix, int currentObjectNum) {
	this->currentObjectNum = currentObjectNum;
	if (animState == STATE_PLAYING) {
		UpdateBoneMatrix_u(&rootNode, WorldMatrix);
		if (playCount[currentObjectNum] <= 0) {
			animState = STATE_IDLE;
			return;
		}
		currentAnimTime[this->currentObjectNum] += timeElapsed;
	}
	else if (animState == STATE_SWITCHING) {
		UpdateBoneMatrix_c(&rootNode, WorldMatrix);
		if (interpolateFactor[currentObjectNum] > 1.0f) {
			interpolateFactor[currentObjectNum] = 0;
			animState = STATE_PLAYING;
			return;
		}
		interpolateFactor[currentObjectNum] += 0.1f;
	}
	else if (animState == STATE_IDLE) {
		Record(this->currentObjectNum);
		Reset(this->currentObjectNum);
		PlayAnim("Idle", this->currentObjectNum, 100);
	}
}

void Model_Scene_Animated::UpdateBoneMatrix_u(Model_Node *node, mat4 &ParentModelMatrix) {

	Model_NodeAnim *nodeAnim = FindNodeAnim(node->name);

	if (nodeAnim != nullptr) {
		float currentTick = (float)currentAnimTime[currentObjectNum] * INV_ONE_MS * currentAnim[currentObjectNum]->tickPerSec;
		if (currentTick > currentAnim[currentObjectNum]->duration) {
			currentTick -= currentAnim[currentObjectNum]->duration;
			currentAnimTime[currentObjectNum] = 0;
			if(playCount[currentObjectNum] < 100)
				playCount[currentObjectNum] -= 1;
		}
		node->transformation_u[currentObjectNum] = ParentModelMatrix * nodeAnim->CalcTransform(currentTick, currentAnim[currentObjectNum]->duration);
	}
	else {
		node->transformation_u[currentObjectNum] = ParentModelMatrix * node->transformation;
	}

	for (int i = 0; i < numMeshes; i++) {
		int boneIndex;
		if (mesh[i].boneMap.find(node->name.data()) != mesh[i].boneMap.end()) {
			boneIndex = mesh[i].boneMap[node->name.data()];
			mesh[i].boneMatrix_u[currentObjectNum][boneIndex] = 
				GlobalInvMatrix * node->transformation_u[currentObjectNum] * mesh[i].boneMatrix[boneIndex];
			break;
		}
	}

	for (int i = 0; i < node->numChild; i++) {
		UpdateBoneMatrix_u(&node->children[i], node->transformation_u[currentObjectNum]);
	}
}

void Model_Scene_Animated::UpdateBoneMatrix_c(Model_Node *node, mat4 &ParentModelMatrix) {

	Model_NodeAnim *nodeAnim_c = FindNodeAnim_c(node->name);
	Model_NodeAnim *nodeAnim = FindNodeAnim(node->name);

	if (nodeAnim_c != nullptr) {
		node->transformation_u[currentObjectNum] = ParentModelMatrix * nodeAnim_c->CalcTransform_c(currentObjectNum, nodeAnim, interpolateFactor[currentObjectNum]);
	}
	else {
		node->transformation_u[currentObjectNum] = ParentModelMatrix * node->transformation;
	}

	for (int i = 0; i < numMeshes; i++) {
		int boneIndex;
		if (mesh[i].boneMap.find(node->name.data()) != mesh[i].boneMap.end()) {
			boneIndex = mesh[i].boneMap[node->name.data()];
			mesh[i].boneMatrix_u[currentObjectNum][boneIndex] =
				GlobalInvMatrix * node->transformation_u[currentObjectNum] * mesh[i].boneMatrix[boneIndex];
			break;
		}
	}

	for (int i = 0; i < node->numChild; i++) {
		UpdateBoneMatrix_c(&node->children[i], node->transformation_u[currentObjectNum]);
	}
}

void Model_Scene_Animated::Record(const int &objectNum) {
	currentObjectNum = objectNum;
	recentAnim[objectNum] = currentAnim[objectNum];
	Record_u(&rootNode);
}

void Model_Scene_Animated::Record_u(Model_Node *node) {
	Model_NodeAnim *nodeAnim = FindNodeAnim(node->name);

	if (nodeAnim != nullptr) {
		float currentTick = (float)currentAnimTime[currentObjectNum] * INV_ONE_MS * currentAnim[currentObjectNum]->tickPerSec;
		nodeAnim->RecordKeys(currentObjectNum, currentTick, currentAnim[currentObjectNum]->duration);
	}

	for (int i = 0; i < node->numChild; i++) {
		Record_u(&node->children[i]);
	}
}

Model_Animation* Model_Scene_Animated::FindAnim(string animName) {
	for (int i = 0; i < numAnims; i++) {
		if (animations[i].name.compare(animName) == 0) {
			return &animations[i];
		}
	}
	return nullptr;
}

Model_NodeAnim* Model_Scene_Animated::FindNodeAnim(string animName) {
	if (currentAnim[currentObjectNum] == nullptr)
		return nullptr;

	for (int i = 0; i < currentAnim[currentObjectNum]->numChannels; i++) {
		if (currentAnim[currentObjectNum]->channels[i].name.compare(animName) == 0) {
			return &currentAnim[currentObjectNum]->channels[i];
		}
	}
	return nullptr;
}

Model_NodeAnim* Model_Scene_Animated::FindNodeAnim_c(string animName) {
	if (recentAnim[currentObjectNum] == nullptr)
		return nullptr;

	for (int i = 0; i < recentAnim[currentObjectNum]->numChannels; i++) {
		if (recentAnim[currentObjectNum]->channels[i].name.compare(animName) == 0) {
			return &recentAnim[currentObjectNum]->channels[i];
		}
	}
	return nullptr;
}

void Model_Scene_Animated::GenObjects() {
	vao_model.resize(mesh.size());
	vbo_model_vertex.resize(mesh.size());
	vbo_model_normal.resize(mesh.size());
	vbo_model_uv.resize(mesh.size());
	vbo_model_weight.resize(mesh.size());
	vbo_model_boneID.resize(mesh.size());
	glGenVertexArrays(vao_model.size(), &vao_model[0]);
	glGenBuffers(vbo_model_vertex.size(), &vbo_model_vertex[0]);
	glGenBuffers(vbo_model_normal.size(), &vbo_model_normal[0]);
	glGenBuffers(vbo_model_uv.size(), &vbo_model_uv[0]);
	glGenBuffers(vbo_model_weight.size(), &vbo_model_weight[0]);
	glGenBuffers(vbo_model_boneID.size(), &vbo_model_boneID[0]);

	vao_selectionBox.resize(1);
	vbo_selectionBox_vertex.resize(1);
	glGenVertexArrays(1, &vao_selectionBox[0]);
	glGenBuffers(1, &vbo_selectionBox_vertex[0]);
}

void Model_Scene_Animated::BindObjectData() {
	for (int i = 0; i < vao_model.size(); i++) {
		glBindVertexArray(vao_model[i]);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo_model_vertex[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].vertex_model.size() * sizeof(vec3), &mesh[i].vertex_model[0][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_model_normal[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].normal_model.size() * sizeof(vec3), &mesh[i].normal_model[0][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_model_uv[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].uv_model.size() * sizeof(vec2), &mesh[i].uv_model[0][0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(2);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo_model_weight[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].weight.size() * sizeof(float), &mesh[i].weight[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, MAX_WEIGHT_BONE_NUMBER, GL_FLOAT, GL_FALSE, MAX_WEIGHT_BONE_NUMBER * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_model_boneID[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].boneID.size() * sizeof(int), &mesh[i].boneID[0], GL_STATIC_DRAW);
		glVertexAttribIPointer(4, MAX_WEIGHT_BONE_NUMBER, GL_INT, MAX_WEIGHT_BONE_NUMBER * sizeof(int), (GLvoid*)0);
		glEnableVertexAttribArray(4);
	}

	glBindVertexArray(vao_selectionBox[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_selectionBox_vertex[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex_selectionBox.size() * sizeof(vec3), &vertex_selectionBox[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
}