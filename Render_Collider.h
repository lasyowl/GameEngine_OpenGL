#ifndef __RENDER_COLLIDER_H
#define __RENDER_COLLIDER_H

#include "Mesh_Sphere.h"

#include <glm/glm.hpp>
#include <gl/glew.h>

class Render;
class Render_Sphere;

enum RendererEnum { RENDERER_SPHERE, RENDERER_SIZE };
enum RendererVariable { VARIABLE_RADIUS };

class Render_Collider {
public:
	Render_Collider();
	~Render_Collider();

	void SetRenderer(Render *renderer, const int &rendererEnum);
	void SelectRenderer(const int &rendererEnum);
	void SetVariables(const int &variableEnum, const float &value);
	void Draw(const glm::mat4 &ViewProjectionMatrix);
	void Initiate();

private:
	void GenSphereObject();

	Render_Sphere *render_sphere;
	int currentRenderer;
	GLuint vao[RENDERER_SIZE];
	Mesh_Sphere mesh_sphere;
	GLuint vbo_vertex[RENDERER_SIZE];
	GLuint index[RENDERER_SIZE];
};
#endif