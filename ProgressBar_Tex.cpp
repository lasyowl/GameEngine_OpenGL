#include "ProgressBar_Tex.h"

ProgressBar_Tex::ProgressBar_Tex() {
	for (int i = 0; i < NUM_PB_MESH; i++) {
		tbo[i] = -1;
	}
}

ProgressBar_Tex::~ProgressBar_Tex() {
	glDeleteTextures(NUM_PB_MESH, tbo);
}