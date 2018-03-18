#ifndef __PROGRESSBAR_TEX_H
#define __PROGRESSBAR_TEX_H

#include <gl/glew.h>

#ifndef NUM_PB_MESH
#define NUM_PB_MESH 4
#endif

class ProgressBar_Tex {
public:
	ProgressBar_Tex();
	~ProgressBar_Tex();

	GLuint tbo[NUM_PB_MESH];

private:

};
#endif