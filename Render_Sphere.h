#ifndef __RENDER_SPHERE_H
#define __RENDER_SPHERE_H

#include <vector>

#include "Render.h"
#include "Mesh_Sphere.h"

typedef unsigned int GLuint;

class Render_Sphere : public Render {

public:
	Render_Sphere();
	~Render_Sphere();

	void SetRadius(const float &radius);
	void Draw(const glm::mat4 &ViewProjectionMatrix, const GLuint &vao, const int &indexCount);
	void Initiate();

	bool highlight;

private:
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();

	float radius;
};
#endif