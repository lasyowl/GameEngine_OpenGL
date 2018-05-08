#ifndef __RENDER_MODELS_STATIC_H
#define __RENDER_MODELS_STATIC_H

#include <list>

#include "Render.h"
#include "Model_Scene_Static.h"
#include "GlobalFunctions.h"
#include "Object_Info.h"

class Light;

class Render_Models_Static : public Render{

public:
	Render_Models_Static();
	~Render_Models_Static();

	void Draw(glm::mat4 ViewProjecitonMatrix);
	void DrawSelectionBox();
	void InitModelRenderer();
	std::list<ObjectInfo>::iterator AddObject(const ObjectInfo &objectInfo);
	void UpdateObject(std::list<ObjectInfo>::iterator objectIter, const glm::mat4 &TransformMatrix);
	void DeleteObject(std::list<ObjectInfo>::iterator objectIter);
	std::list<ObjectInfo>::iterator IsFull();
	void SetLight(Light *light);
	void SetDrawRadius(const float &radius);
	void UpdateEyePos(const glm::vec3 &eyePosition);
	void UpdatePlayerPos(const glm::vec3 &playerPos);

	std::vector<Model_Scene_Static> scene;

private:
	void LoadModelData();
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();
	void FakeLight(const std::list<ObjectInfo>::iterator &iter);
	void SetShaderVar_SelectionBox();
	void DrawNodeRecursive(Model_Node *node, glm::mat4 &ParentModelMatrix);

	GLuint texture_particle;
	GLuint randTexture;

	Light *light;
	Model_Scene_Static *currentScene;
	Model_Material *currentMaterial;
	int numScene;
	int currentObjectNum;
	std::list<ObjectInfo> objectInfo;
	std::list<int> objectIndex;
	glm::vec3 playerPos;
	float radius;

	GlobalFunctions gf;
};
#endif