#ifndef __RENDER_MODELS_ANIMATED_H
#define __RENDER_MODELS_ANIMATED_H

#include <list>

#include "Render.h"
#include "Model_Scene_Animated.h"
#include "GlobalFunctions.h"
#include "Object_Info.h"

class Light;
class Framebuffer;

class Render_Models_Animated : public Render{
public:
	Render_Models_Animated();
	~Render_Models_Animated();

	void InitModelRenderer();
	void DrawModels(glm::mat4 ViewProjecitonMatrix, int timeElapsed);
	void DrawSelectionBox();
	std::list<ObjectInfo>::iterator AddObject(const ObjectInfo &objectInfo);
	void UpdateObject(std::list<ObjectInfo>::iterator objectIter, const glm::mat4 &TransformMatrix);
	void DeleteObject(std::list<ObjectInfo>::iterator objectIter);
	void SetLight(Light *light);
	void SetDrawRadius(const float &radius);
	void UpdateEyePos(const glm::vec3 &eyePosition);
	void UpdatePlayerPos(const glm::vec3 &playerPos);
	void SetFramebuffer(Framebuffer *framebuffer);
	std::list<ObjectInfo>::iterator IsFull();
	void PlayAnim(const std::list<ObjectInfo>::iterator &iter, const int &playCount);

	std::vector<Model_Scene_Animated> scene;

private:
	void LoadModelData();
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();
	void SetShaderVar_SelectionBox();
	void DrawNodeRecursive(Model_Node *node);
	void DrawNodeRecursive_SelectionBox(Model_Node *node);
	void SetBoneMatrix(Model_Mesh_Animated *mesh);

	GLuint texture_particle;
	GLuint randTexture;

	Light *light;
	Model_Scene_Animated *currentScene;
	Model_Material *currentMaterial;
	int numScene;
	bool useBone;
	int currentObjectNum;
	glm::vec3 playerPos;
	float radius;
	std::list<ObjectInfo> objectInfo;
	std::list<int> objectIndex;
	Framebuffer *framebuffer;

	GlobalFunctions gf;
};
#endif