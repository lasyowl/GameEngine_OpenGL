#ifndef __SUN_H
#define __SUN_H

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Mesh_Quad.h"

class Sun {
public:
	Sun();
	~Sun();

	void SetPosition(glm::vec3 *position);
	void Initiate();
	
	glm::vec3 *position;
	GLuint vao;
	GLuint tex_core;
	GLuint tex_lens[9];

private:
	void GenObjects();
	void BindObjectData();

	Mesh_Quad mesh;
	GLuint vbo_vertex;
	GLuint vbo_uv;
};
#endif