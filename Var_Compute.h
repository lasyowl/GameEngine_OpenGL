#ifndef __VAR_COMPUTE_H
#define __VAR_COMPUTE_H

typedef unsigned int GLuint;

class Var_Compute {

public:
	Var_Compute();
	~Var_Compute();

	GLuint loc_texture_heightmap;
	GLuint loc_texture_random;
	GLuint loc_heightmapWidthHeight;
	GLuint loc_particle_size;

private:

};
#endif