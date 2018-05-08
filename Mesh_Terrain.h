#ifndef __MESH_TERRAIN_H
#define __MESH_TERRAIN_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

#define ERR_RAYCAST -1

typedef unsigned char GLubyte;
typedef unsigned int GLuint;

class Var_Compute;
class Parse_Shader;

// Class Mesh_Terrain generates n * n square mesh by triangle strip.
class Mesh_Terrain {

public:
	Mesh_Terrain();
	Mesh_Terrain(const int &width, const glm::vec2 &offset);
	~Mesh_Terrain();

	void Initiate();
	void SetDimension(const int &width, const glm::vec2 &offset);
	void SetHeightmap(const std::string &fileName);
	GLuint GetVao();
	glm::vec3 GetRaycastCoord_MP(const glm::vec3 &origin, const glm::vec3 &dir, const float &range, const int &iteration);
	glm::vec3 GetRaycastCoord_S(const glm::vec3 &origin, const glm::vec3 &dir, const float &delta, const float &range);
	float GetHeight(const glm::vec3 &objectPosition);
	void RefreshVertex();

	std::vector<glm::vec4> vertex;
	std::vector<GLubyte> alphamap;
	int width;

private:
	void GenMeshVertex();
	void GenVariables();
	void GenObjects();
	void DispatchMesh();
	void BindObjects();
	void GenShader();
	void LoadTexture(const char *filePath);
	void LoadTextureArray(const char *filePath, int z);

	glm::vec2 offset;
	std::vector<glm::vec4> normal;
	GLuint vao;
	GLuint vbo_vertex;
	GLuint vbo_normal;
	GLuint shader_compute;
	GLuint tbo_heightmap;
	std::string filePath;
	std::string fileName_heightmap;
	std::string fullPath;

	Var_Compute *varCompute;
	Parse_Shader *parse;
};
#endif