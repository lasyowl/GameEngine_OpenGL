#ifndef __LOADER_TEXTURE_H
#define __LOADER_TEXTURE_H

#include <gl/glew.h>

class Loader_Texture {
public:
	Loader_Texture();
	~Loader_Texture();

	void LoadTexture(const char *filePath);
	void LoadTextureArray(const char *filePath, int z);

private:

};
#endif