#include "Texture_Terrain.h"
#include "Mesh_Terrain.h"

Texture_Terrain::Texture_Terrain() {

}

Texture_Terrain::~Texture_Terrain() {

}

void Texture_Terrain::SetMesh(Mesh_Terrain *mesh_terrain) {
	this->mesh = mesh_terrain;
}

void Texture_Terrain::GenTextures() {
	filePath_alphamap = "Res/Heightmap/Alpha_save.png";

	numTerrainTex = 5;

	glGenTextures(1, &texture_terrainPack);
	glGenTextures(1, &texture_normalPack);
	glGenTextures(1, &texture_specularPack);
	glGenTextures(1, &texture_alphamap);

	glActiveTexture(GL_TEXTURE0 + texture_terrainPack);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_terrainPack);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 5, GL_RGBA32F, 1024, 1024, numTerrainTex);
	LoadTextureArray("Res/Terrain/ground_mud_d.jpg", 0);
	LoadTextureArray("Res/Terrain/island_sand_d.jpg", 1);
	LoadTextureArray("Res/Terrain/grass_green_d.jpg", 2);
	LoadTextureArray("Res/Terrain/snow_bumpy_d.jpg", 3);
	LoadTextureArray("Res/Terrain/mntn_black_d.jpg", 4);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, -2.4f);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glActiveTexture(GL_TEXTURE0 + texture_normalPack);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_normalPack);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 5, GL_RGBA32F, 1024, 1024, numTerrainTex);
	LoadTextureArray("Res/Terrain/ground_mud_n.jpg", 0);
	LoadTextureArray("Res/Terrain/island_sand_n.jpg", 1);
	LoadTextureArray("Res/Terrain/grass_green_n.jpg", 2);
	LoadTextureArray("Res/Terrain/snow_bumpy_n.jpg", 3);
	LoadTextureArray("Res/Terrain/mntn_black_n.jpg", 4);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, -2.4f);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glActiveTexture(GL_TEXTURE0 + texture_specularPack);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_specularPack);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 5, GL_RGBA32F, 1024, 1024, numTerrainTex);
	LoadTextureArray("Res/Terrain/ground_mud_s.jpg", 0);
	LoadTextureArray("Res/Terrain/island_sand_s.jpg", 1);
	LoadTextureArray("Res/Terrain/grass_green_s.jpg", 2);
	LoadTextureArray("Res/Terrain/snow_bumpy_s.jpg", 3);
	LoadTextureArray("Res/Terrain/mntn_black_s.jpg", 4);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, -0.4f);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glActiveTexture(GL_TEXTURE0 + texture_alphamap);
	glBindTexture(GL_TEXTURE_2D, texture_alphamap);
	LoadTexture(filePath_alphamap.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture_Terrain::RefreshAlphamap() {
	glActiveTexture(GL_TEXTURE0 + texture_alphamap);
	glBindTexture(GL_TEXTURE_2D, texture_alphamap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mesh->width, mesh->width, 0, GL_RGBA, GL_UNSIGNED_BYTE, &mesh->alphamap[0]);

	glGenerateMipmap(GL_TEXTURE_2D);
}
