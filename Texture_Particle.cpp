#include "Texture_Particle.h"

#include "Loader_Texture.h"
#include <stdio.h>
Texture_Particle::Texture_Particle() {
	
}

Texture_Particle::~Texture_Particle() {

}

void Texture_Particle::Initiate(const char *filePath, const int &numRow, const int &numCol) {
	this->numRow = numRow;
	this->numCol = numCol;
	numSprites = numRow * numCol;

	Loader_Texture loader;

	glGenTextures(1, &tbo);
	glActiveTexture(GL_TEXTURE0 + tbo);
	glBindTexture(GL_TEXTURE_2D, tbo);
	loader.LoadTexture(filePath);
	glGenerateMipmap(GL_TEXTURE_2D);
}