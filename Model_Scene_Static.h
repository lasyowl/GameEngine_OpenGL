#ifndef __MODEL_SCENE_STATIC_H
#define __MODEL_SCENE_STATIC_H

#include <string>
#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Model_Mesh_Static.h"
#include "Model_Node.h"
#include "Model_Material.h"
#include "Model_List.h"

class Model_Scene_Static {
public:
	Model_Scene_Static();
	~Model_Scene_Static();

	void BindVao(int index);
	void BindVao_SelectionBox(int index);
	void LoadScene(const FileInfo &fileInfo);
	int GetNumVao();
	int GetNumVertex(int index);

	Model_Node rootNode;
	std::vector<Model_Mesh_Static> mesh;
	int numMeshes;
	std::vector<int> numVertex;
	int numMaterials;
	std::vector<Model_Material> materials;

	SelectionBox selectionBox;

private:
	void GenObjects();
	void GenSelectionBoxVertice();
	void BindObjectData();
	void NodePostProcess(Model_Node *node);
	void LoadTexture(const char *filePath);

	std::vector<GLuint> vao;
	std::vector<GLuint> vbo_vertex;
	std::vector<GLuint> vbo_normal;
	std::vector<GLuint> vbo_uv;

	std::vector<GLuint> vao_selectionBox;
	std::vector<GLuint> vbo_selectionBox_vertex;
	std::vector<glm::vec3> vertex_selectionBox;
};
#endif