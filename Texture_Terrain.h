#ifndef __TEXTURE_TERRAIN_H
#define __TEXTURE_TERRAIN_H

#include <GL/glew.h>
#include <string>
#include "Render.h"

class Mesh_Terrain;

class Texture_Terrain : public Render{
public:
	Texture_Terrain();
	~Texture_Terrain();

	void SetMesh(Mesh_Terrain *mesh_terrain);
	void GenTextures();
	void RefreshAlphamap();

	int numTerrainTex;
	int numAlphaTex;
	GLuint texture_heightmap;
	GLuint texture_alphamap;
	GLuint texture_terrainPack;
	GLuint texture_normalPack;
	GLuint texture_specularPack;
	std::string filePath_alphamap;

	Mesh_Terrain *mesh;

private:
	
};
#endif