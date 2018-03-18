#ifndef __EDITOR_TERRAIN_H
#define __EDITOR_TERRAIN_H

#include <vector>
#include <glm/glm.hpp>

#include "Texture_Terrain.h"

class Mesh_Terrain;

struct Brush_Height {
	std::vector<std::vector<int> > texVal;
	GLuint tex_brush;
};

class Editor_Terrain {
public:
	Editor_Terrain();
	~Editor_Terrain();

	void SetTargetMesh(Mesh_Terrain *mesh);
	void SetTexture_Terrrain(Texture_Terrain *texture_terrain);
	void SetViewport(glm::vec2 *viewport);
	void Initiate();
	void SetBrush(const int &brush);
	void SetTexture(const int &texture);
	void SetBrushSize(const float &brushSize);
	void AdjustHeight(const glm::vec3 &center, const bool &positive);
	void AdjustTexture(const glm::vec3 &center);
	void SaveAlphamap(const char *filePath);
	void SaveHeightmap(const char *filePath);
	std::vector<Brush_Height> brush_height;
	float brushSize;
	float opacity;
	bool textureMode;
	Texture_Terrain *texture_terrain;

private:
	void LoadTextureToArray_Grayscale(std::vector<std::vector<int> > *target, const char *filePath);
	void LoadTextureToArray_Alphamap(std::vector<GLubyte> *target, const char *filePath);
	int currentBrush;
	int currentTexture;

	GLubyte *data;
	int width, height;
	Mesh_Terrain *mesh;
	glm::vec2 *viewport;
};
#endif