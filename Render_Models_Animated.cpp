#include <glm/gtc/matrix_transform.hpp>

#include "Parse_Shader.h"
#include "Render_Models_Animated.h"
#include "Model_List.h"
#include "Light.h"
#include "Var_Render.h"
#include "Framebuffer.h"

using namespace std;
using namespace glm;

Render_Models_Animated::Render_Models_Animated() : useBone(false) {
	varRender = new Var_Render;
	varRender_selectionBox = new Var_Render;
}

Render_Models_Animated::~Render_Models_Animated() {
	delete varRender;
	delete varRender_selectionBox;
}

void Render_Models_Animated::InitModelRenderer() {
	LoadModelData();
	GenShaderProgram();
	GetShaderVar();
}

void Render_Models_Animated::DrawModels(mat4 ViewProjectionMatrix, int timeElapsed) {
	varRender->ViewProjectionMatrix = ViewProjectionMatrix;
	glUseProgram(shaderProgram[0]);
	list<ObjectInfo>::iterator iter = objectInfo.begin();
	while (iter != objectInfo.end()) {
		currentObjectNum = iter->charIndex;
		currentScene = &scene[iter->modelId];
		iter->HandMatrix = scene[iter->modelId].HandMatrix[currentObjectNum];
		if (distance(playerPos, iter->pos) < radius) {
			if (timeElapsed != 0) 
				currentScene->UpdateBoneMatrix(timeElapsed, iter->TransformMatrix, currentObjectNum);

			DrawNodeRecursive(&currentScene->rootNode);
		}
		++iter;
	}
}

void Render_Models_Animated::DrawNodeRecursive(Model_Node *node) {
	varRender->ModelMatrix = node->transformation_u[currentObjectNum];

	for (int i = 0; i < node->numMesh; i++) {
		Model_Mesh_Animated *currentMesh = &currentScene->mesh[node->meshIndex[i]];

		if (currentMesh->numBoneMatrix > 0)
			useBone = true;
		else useBone = false;

		currentMaterial = &currentScene->materials[currentMesh->materialIndex];

		SetBoneMatrix(currentMesh);
		SetShaderVar();
		
		currentScene->BindVao_Model(node->meshIndex[i]);
		glDrawArrays(GL_TRIANGLES, 0, currentScene->GetNumVertex(node->meshIndex[i]));
	}
	for (int i = 0; i < node->numChild; i++) {
		DrawNodeRecursive(&node->children[i]);
	}
}

void Render_Models_Animated::DrawSelectionBox() {
	glUseProgram(shaderProgram[1]);
	list<ObjectInfo>::iterator iter = objectInfo.begin();
	while (iter != objectInfo.end()) {
		if (distance(playerPos, iter->pos) < radius) {
			currentObjectNum = iter->charIndex;
			currentScene = &scene[iter->modelId];
			varRender->ModelMatrix = iter->SelectionBoxTransformMatrix * currentScene->rootNode.transformation;
			currentScene->BindVao_SelectionBox(0);
			if (iter->isSelected)
				varRender_selectionBox->color_line = vec3(1, 0, 0);
			else varRender_selectionBox->color_line = vec3(0, 1, 0);
			SetShaderVar_SelectionBox();
			glDrawArrays(GL_LINE_STRIP, 0, 16);
			//DrawNodeRecursive_SelectionBox(&currentScene->rootNode);
		}
		++iter;
	}
}

void Render_Models_Animated::DrawNodeRecursive_SelectionBox(Model_Node *node) {
	SetShaderVar_SelectionBox();
	for (int i = 0; i < node->numMesh; i++) {
		currentScene->BindVao_SelectionBox(node->meshIndex[i]);
		glDrawArrays(GL_LINE_STRIP, 0, 16);
	}
	for (int i = 0; i < node->numChild; i++) {
		DrawNodeRecursive_SelectionBox(&node->children[i]);
	}
}

void Render_Models_Animated::LoadModelData() {
	Model_List modelList;
	modelList.ReadFile("Res/Models/ModelList_Animated.mdl");

	scene.resize(modelList.modelList.size());
	for (int i = 0; i < modelList.modelList.size(); i++) {
		scene[i].LoadScene(modelList.modelList[i]);
	}
	numScene = scene.size();
	for (int i = 0; i < MAX_OBJECT_NUM; i++) {
		objectIndex.push_back(i);
	}
}

void Render_Models_Animated::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Model_Animated.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Model_Animated.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};
	ShaderInfo shaderInfo_selectionBox[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/SelectionBox.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/SelectionBox.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_selectionBox));
}

void Render_Models_Animated::GetShaderVar() {
	glUseProgram(shaderProgram[0]);
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ViewProjectionMatrix");
	varRender->loc_ModelMatrix = glGetUniformLocation(shaderProgram[0], "ModelMatrix");
	varRender->loc_InvTransposeMatrix = glGetUniformLocation(shaderProgram[0], "InvTransposeMatrix");
	varRender->loc_GlobalInvMatrix = glGetUniformLocation(shaderProgram[0], "GlobalInvMatrix");
	varRender->loc_BoneMatrix = glGetUniformLocation(shaderProgram[0], "BoneMatrix");
	varRender->loc_useBone = glGetUniformLocation(shaderProgram[0], "useBone"); 
	
	varRender->loc_numLight = glGetUniformLocation(shaderProgram[0], "numLight");
	varRender->loc_light_intensity = glGetUniformLocation(shaderProgram[0], "light_intensity");
	varRender->loc_light_position = glGetUniformLocation(shaderProgram[0], "light_position");
	varRender->loc_light_direction = glGetUniformLocation(shaderProgram[0], "light_direction");
	varRender->loc_color_ambient = glGetUniformLocation(shaderProgram[0], "color_ambient");
	varRender->loc_color_diffuse = glGetUniformLocation(shaderProgram[0], "color_diffuse");
	varRender->loc_color_specular = glGetUniformLocation(shaderProgram[0], "color_specular");
	varRender->loc_texture_diffuse = glGetUniformLocation(shaderProgram[0], "texture_diffuse");
	varRender->loc_texture_normalmap = glGetUniformLocation(shaderProgram[0], "texture_normalmap");
	varRender->loc_tex_shadowmap = glGetUniformLocation(shaderProgram[0], "tex_shadowmap");
	varRender->loc_eye_position = glGetUniformLocation(shaderProgram[0], "eye_position");
	varRender->loc_useTexture = glGetUniformLocation(shaderProgram[0], "useTexture");
	varRender->loc_useNormalmap = glGetUniformLocation(shaderProgram[0], "useNormalmap");

	glUniform1i(varRender->loc_tex_shadowmap, framebuffer->depthTex_shadowmap);

	glUseProgram(shaderProgram[1]);
	varRender_selectionBox->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[1], "ViewProjectionMatrix");
	varRender_selectionBox->loc_ModelMatrix = glGetUniformLocation(shaderProgram[1], "ModelMatrix");
	varRender_selectionBox->loc_color_line = glGetUniformLocation(shaderProgram[1], "color_line");
}

void Render_Models_Animated::SetBoneMatrix(Model_Mesh_Animated *mesh) {
	varRender->numBoneMatrix = mesh->numBoneMatrix;
	if (mesh->numBoneMatrix > 0)
		varRender->BoneMatrix = &mesh->boneMatrix_u[currentObjectNum][0];
}

void Render_Models_Animated::SetFramebuffer(Framebuffer *framebuffer) {
	this->framebuffer = framebuffer;
}

void Render_Models_Animated::SetShaderVar() {
	// Model variables
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(varRender->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(varRender->loc_ModelMatrix, 1, GL_FALSE, &varRender->ModelMatrix[0][0]);
	glUniformMatrix3fv(varRender->loc_InvTransposeMatrix, 1, GL_FALSE, &varRender->InvTransposeMatrix[0][0]);
	glUniformMatrix4fv(varRender->loc_GlobalInvMatrix, 1, GL_FALSE, &currentScene->GlobalInvMatrix[0][0]);
	glUniformMatrix4fv(varRender->loc_BoneMatrix, varRender->numBoneMatrix, GL_FALSE, &varRender->BoneMatrix[0][0][0]);
	if (useBone == true) 
		glUniform1i(varRender->loc_useBone, true);
	else glUniform1i(varRender->loc_useBone, false);

	glUniform1i(varRender->loc_numLight, light->numLight);
	glUniform1fv(varRender->loc_light_intensity, light->numLight, &light->intensity[0]);
	glUniform3fv(varRender->loc_light_position, light->numLight, &light->position[0][0]);
	glUniform3fv(varRender->loc_light_direction, light->numLight, &light->direction[0][0]);
	glUniform3fv(varRender->loc_eye_position, 1, &varRender->eyePosition[0]);
	glUniform4fv(varRender->loc_color_ambient, 1, &currentMaterial->color_ambient[0]);
	glUniform4fv(varRender->loc_color_diffuse, 1, &currentMaterial->color_diffuse[0]);
	glUniform4fv(varRender->loc_color_specular, 1, &currentMaterial->color_specular[0]);
	glUniform1i(varRender->loc_texture_diffuse, currentMaterial->tbo_diffuse);
	glUniform1i(varRender->loc_useTexture, currentMaterial->useTexture);
	if (currentMaterial->tbo_normal == -1) {
		glUniform1i(varRender->loc_useNormalmap, false);
	}
	else {
		glUniform1i(varRender->loc_texture_normalmap, currentMaterial->tbo_normal);
		glUniform1i(varRender->loc_useNormalmap, true);
	}
}

void Render_Models_Animated::SetShaderVar_SelectionBox() {
	// Selection-box variables
	glUniformMatrix4fv(varRender_selectionBox->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(varRender_selectionBox->loc_ModelMatrix, 1, GL_FALSE, &varRender->ModelMatrix[0][0]);
	glUniform3fv(varRender_selectionBox->loc_color_line, 1, &varRender_selectionBox->color_line[0]);
}

void Render_Models_Animated::SetLight(Light *light) {
	this->light = light;
}

void Render_Models_Animated::SetDrawRadius(const float &radius) {
	this->radius = radius;
}

void Render_Models_Animated::UpdatePlayerPos(const vec3 &playerPos) {
	this->playerPos = playerPos;
}
void Render_Models_Animated::UpdateEyePos(const vec3 &eyePosition) {
	varRender->eyePosition = eyePosition;
}

void Render_Models_Animated::UpdateObject(list<ObjectInfo>::iterator objectIter, const glm::mat4 &TransformMatrix) {
	objectIter->TransformMatrix = TransformMatrix;
}

list<ObjectInfo>::iterator Render_Models_Animated::AddObject(const ObjectInfo &objectInfo) {
	if (objectIndex.empty())
		return this->objectInfo.end();

	list<ObjectInfo>::iterator iter = this->objectInfo.end();
	this->objectInfo.insert(iter, objectInfo);
	--iter;
	iter->selectionBox = &scene[objectInfo.modelId].selectionBox;
	iter->charIndex = *objectIndex.begin();
	printf("Inserted object %d\n", iter->charIndex);
	objectIndex.erase(objectIndex.begin());

	return iter;
}

std::list<ObjectInfo>::iterator Render_Models_Animated::IsFull() {
	return objectInfo.end();
}

void Render_Models_Animated::DeleteObject(list<ObjectInfo>::iterator objectIter) {
	printf("Deleted animated object %d\n", objectIter->charIndex);
	objectIndex.push_back(objectIter->charIndex);
	objectInfo.erase(objectIter);
}

int Render_Models_Animated::PlayAnim(const list<ObjectInfo>::iterator &iter, const int &playCount, const bool &loopAnim) {
	scene[iter->modelId].Record(iter->charIndex);
	scene[iter->modelId].Reset(iter->charIndex);
	return scene[iter->modelId].PlayAnim(iter->currentAnim, iter->charIndex, playCount, loopAnim);
}