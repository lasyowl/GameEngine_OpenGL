#include <glm/gtc/matrix_transform.hpp>

#include "Parse_Shader.h"
#include "Render_Models_Static.h"
#include "Model_List.h"
#include "Light.h"
#include "Var_Render.h"

using namespace std;
using namespace glm;

Render_Models_Static::Render_Models_Static() {
	varRender = new Var_Render;
	varRender_selectionBox = new Var_Render;
}

Render_Models_Static::~Render_Models_Static() {
	delete varRender;
	delete varRender_selectionBox;
}

void Render_Models_Static::InitModelRenderer() {
	LoadModelData();
	GenShaderProgram();
	GetShaderVar();
}

void Render_Models_Static::Draw(mat4 ViewProjectionMatrix) {
	varRender->ViewProjectionMatrix = ViewProjectionMatrix;
	glUseProgram(shaderProgram[0]);
	list<ObjectInfo>::iterator iter = objectInfo.begin();
	while (iter != objectInfo.end()) {
		currentObjectNum = iter->charIndex;
		currentScene = &scene[iter->modelId];
		FakeLight(iter);
		if(distance(playerPos, iter->pos) < radius)
			DrawNodeRecursive(&currentScene->rootNode, iter->TransformMatrix);
		iter++;
	}
}

void Render_Models_Static::DrawNodeRecursive(Model_Node *node, mat4 &ParentModelMatrix) {
	mat4 CurrentModelMatrix = ParentModelMatrix * node->transformation;
	varRender->ModelMatrix = CurrentModelMatrix;
	
	for (int i = 0; i < node->numMesh; i++) {
		Model_Mesh_Static *currentMesh = &currentScene->mesh[node->meshIndex[i]];

		currentMaterial = &currentScene->materials[currentMesh->materialIndex];

		SetShaderVar();

		currentScene->BindVao(node->meshIndex[i]);
		glDrawArrays(GL_TRIANGLES, 0, currentScene->GetNumVertex(node->meshIndex[i]));
	}
	for (int i = 0; i < node->numChild; i++) {
		DrawNodeRecursive(&node->children[i], CurrentModelMatrix);
	}
}

void Render_Models_Static::DrawSelectionBox() {
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
		}
		iter++;
	}
}

void Render_Models_Static::LoadModelData() {
	Model_List modelList;
	modelList.ReadFile("Res/Models/ModelList_Static.mdl");

	scene.resize(modelList.modelList.size());
	for (int i = 0; i < modelList.modelList.size(); i++) {
		scene[i].LoadScene(modelList.modelList[i]);
	}
	numScene = scene.size();
	for (int i = 0; i < MAX_OBJECT_NUM; i++) {
		objectIndex.push_back(i);
	}
}

void Render_Models_Static::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Model_Static.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Model_Static.frag" },
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

void Render_Models_Static::GetShaderVar() {
	glUseProgram(shaderProgram[0]);
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ViewProjectionMatrix");
	varRender->loc_ModelMatrix = glGetUniformLocation(shaderProgram[0], "ModelMatrix");
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
	varRender->loc_eye_position = glGetUniformLocation(shaderProgram[0], "eye_position");
	varRender->loc_useTexture = glGetUniformLocation(shaderProgram[0], "useTexture");
	varRender->loc_useNormalmap = glGetUniformLocation(shaderProgram[0], "useNormalmap");
	varRender->loc_fakeLighting = glGetUniformLocation(shaderProgram[0], "fakeLighting");

	glUseProgram(shaderProgram[1]);
	varRender_selectionBox->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[1], "ViewProjectionMatrix");
	varRender_selectionBox->loc_ModelMatrix = glGetUniformLocation(shaderProgram[1], "ModelMatrix");
	varRender_selectionBox->loc_color_line = glGetUniformLocation(shaderProgram[1], "color_line");
}

void Render_Models_Static::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(varRender->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(varRender->loc_ModelMatrix, 1, GL_FALSE, &varRender->ModelMatrix[0][0]);

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

void Render_Models_Static::SetShaderVar_SelectionBox() {
	// Selection-box variables
	glUniformMatrix4fv(varRender_selectionBox->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(varRender_selectionBox->loc_ModelMatrix, 1, GL_FALSE, &varRender->ModelMatrix[0][0]);
	glUniform3fv(varRender_selectionBox->loc_color_line, 1, &varRender_selectionBox->color_line[0]);
}

void Render_Models_Static::FakeLight(const list<ObjectInfo>::iterator &iter) {
	if (iter->tag == 1)
		glUniform1i(varRender->loc_fakeLighting, true);
	else glUniform1i(varRender->loc_fakeLighting, false);
}

void Render_Models_Static::SetLight(Light *light) {
	this->light = light;
}

void Render_Models_Static::SetDrawRadius(const float &radius) {
	this->radius = radius;
}

void Render_Models_Static::UpdatePlayerPos(const vec3 &playerPos) {
	this->playerPos = playerPos;
}

void Render_Models_Static::UpdateEyePos(const vec3 &eyePosition) {
	varRender->eyePosition = eyePosition;
}

void Render_Models_Static::UpdateObject(list<ObjectInfo>::iterator objectIter, const glm::mat4 &TransformMatrix) {
	objectIter->TransformMatrix = TransformMatrix;
}

list<ObjectInfo>::iterator Render_Models_Static::AddObject(const ObjectInfo &objectInfo) {
	if (objectIndex.empty())
		return this->objectInfo.end();

	list<ObjectInfo>::iterator iter = this->objectInfo.end();
	this->objectInfo.insert(iter, objectInfo);
	iter--;
	iter->selectionBox = &scene[objectInfo.modelId].selectionBox;
	iter->charIndex = *objectIndex.begin();
	printf("Inserted object %d\n", iter->charIndex);
	objectIndex.erase(objectIndex.begin());

	return iter;
}

std::list<ObjectInfo>::iterator Render_Models_Static::IsFull() {
	return objectInfo.end();
}

void Render_Models_Static::DeleteObject(list<ObjectInfo>::iterator objectIter) {
	printf("Deleted static object %d\n", objectIter->charIndex);
	objectIndex.push_back(objectIter->charIndex);
	objectInfo.erase(objectIter);
}
