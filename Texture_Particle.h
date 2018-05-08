#ifndef __TEXTURE_PARTICLE_H
#define __TEXTURE_PARTICLE_H

#include <gl/glew.h>

class Texture_Particle {
public:
	Texture_Particle();
	~Texture_Particle();

	void Initiate(const char *filePath, const int &numRow, const int &numCol);

	GLuint tbo;
	int numRow;
	int numCol;
	int numSprites;

private:

};
#endif