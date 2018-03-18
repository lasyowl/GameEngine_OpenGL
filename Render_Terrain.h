#ifndef __RENDER_TERRAIN_H
#define __RENDER_TERRAIN_H

#include <vector>

#include "Render.h"
#include "Mesh_Terrain.h"
#include "Texture_Terrain.h"

class Var_Compute;
class Light;
class Framebuffer;
class Texture_Terrain;

#define MAX_LOD_TERRAIN 4
#define NUM_CHUNK_ROW 4
#define ERR_RAYCAST -1
#define DEFAULT_RAYCAST_RANGE 600.0f

typedef unsigned int GLuint;

enum RenderMode { FAST_MODE, MODE_SIZE };

class Render_Terrain : public Render {

public:
	Render_Terrain();
	~Render_Terrain();

	void Initiate(); 
	void SetTerrainWidth(const int &width);
	void SetVP(const glm::mat4 &VP);
	void SetVAO(const GLuint &vao);
	void SetIBO(const GLuint &ibo, const int &numIndex);
	void PrepareDraw();
	void Draw();
	void FinishDraw();
	void SetLight(Light *light);
	void SetFramebuffer(Framebuffer *framebuffer);
	void SetTexture_Terrain(Texture_Terrain *texture_terrain);
	void SetShadowMatrix(const glm::mat4 &ShadowMatrix);
	void UpdateEyePos(const glm::vec3 &eyePosition);
	void EnableMode(const int &mode);
	void DisableMode(const int &mode);

private:
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();
	
	Var_Compute *varCompute;
	Light *light;
	Framebuffer *framebuffer;
	Texture_Terrain *texture_terrain;
	float terrainWidth;
	GLuint vao;
	GLuint ibo;
	int numIndex;
	bool mode[MODE_SIZE];
};
#endif