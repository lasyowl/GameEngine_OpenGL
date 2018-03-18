#ifndef __RENDER_POINT_H
#define __RENDER_POINT_H

#include <vector>
#include <glm/glm.hpp>

#include "Render.h"

class Render_Point : public Render {
public:
	Render_Point();
	~Render_Point();

	void Initiate();
	void UpdatePoint(const glm::vec3 &point);
	void Draw(const glm::mat4 &ViewProjectionMatrix);

private:
	void GenObjects();
	void BindObjects();
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();

	GLuint vao;
	GLuint vbo;
	glm::vec3 point;
};
#endif