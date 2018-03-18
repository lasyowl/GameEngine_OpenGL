#ifndef __MODEL_SCENE_ANIMATED_H
#define __MODEL_SCENE_ANIMATED_H

#include <string>
#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Model_Animation.h"
#include "Model_Mesh_Animated.h"
#include "Model_Node.h"
#include "Model_Material.h"
#include "Model_List.h"

class Model_Scene_Animated {
	enum AnimState { STATE_PLAYING, STATE_IDLE, STATE_OTHERS, STATE_SWITCHING };
public:
	Model_Scene_Animated();
	~Model_Scene_Animated();

	void BindVao_Model(int index);
	void BindVao_SelectionBox(int index);
	void LoadScene(FileInfo &fileInfo);
	void PlayAnim(const std::string &animName, const int &objectNum, const int &playCount);
	void MaintainAnim(const int &objectNum);
	int GetNumVao();
	int GetNumVertex(int index);
	void UpdateBoneMatrix(int timeElapsed, glm::mat4 &WorldMatrix, int currentObjectNum);
	void SetWorldPosition(glm::vec3 &worldPosition);
	void Reset(const int &objectNum);
	void Record(const int &objectNum);

	Model_Node rootNode;
	std::vector<Model_Mesh_Animated> mesh;
	glm::mat4 GlobalInvMatrix;
	int numMeshes;
	std::vector<int> numVertex;
	int numAnims;
	std::vector<Model_Animation> animations;
	int numMaterials;
	std::vector<Model_Material> materials;

	SelectionBox selectionBox;

private:
	void LoadTexture(const char *filePath);
	void GenObjects();
	void BindObjectData();
	void UpdateBoneMatrix_u(Model_Node *node, glm::mat4 &ParentModelMatrix);
	void UpdateBoneMatrix_c(Model_Node *node, glm::mat4 &ParentModelMatrix);
	void Record_u(Model_Node *node);
	Model_Animation *FindAnim(std::string animName);
	Model_NodeAnim *FindNodeAnim(std::string animName);
	Model_NodeAnim *FindNodeAnim_c(std::string animName);
	void NodePostProcess(Model_Node *node);
	void GenSelectionBoxVertice();
	void LoadAdditionalAnims(FileInfo &fileInfo);

	std::vector<GLuint> vao_model;
	std::vector<GLuint> vbo_model_vertex;
	std::vector<GLuint> vbo_model_normal;
	std::vector<GLuint> vbo_model_uv;
	std::vector<GLuint> vbo_model_weight;
	std::vector<GLuint> vbo_model_boneID;

	std::vector<GLuint> vao_selectionBox;
	std::vector<GLuint> vbo_selectionBox_vertex;
	std::vector<glm::vec3> vertex_selectionBox;

	Model_Animation *currentAnim[MAX_OBJECT_NUM];
	Model_Animation *recentAnim[MAX_OBJECT_NUM];
	int currentAnimTime[MAX_OBJECT_NUM];
	float interpolateFactor[MAX_OBJECT_NUM];
	int currentObjectNum;
	int animState;
	int playCount[MAX_OBJECT_NUM];

	glm::vec3 worldPosition;
};
#endif