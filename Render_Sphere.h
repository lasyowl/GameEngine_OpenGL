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

	void Draw(glm::mat4 ViewProjectionMatrix);
	void Initiate();
	void EnableSkyMode();
	void DisableSkyMode();

private:
	void InitSphereRenderer();
	void GenSphereMeshes();
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();
	void GenObjects();
	void BindObjectData();

	Mesh_Sphere sphereMesh;
	GLuint vao;
	GLuint vbo_vertex;
	GLuint vbo_normal;
	GLuint vbo_uv;
	GLuint index;
	GLuint texture;

	bool skyMode;
};
#endif