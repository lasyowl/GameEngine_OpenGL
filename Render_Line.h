#ifndef __RENDER_LINE_H
#define __RENDER_LINE_H

#include <vector>
#include <glm/glm.hpp>
#include <gl/glew.h>

#include "Render.h"

class Render_Line : public Render {
public:
	Render_Line();
	~Render_Line();

	void Initiate();
	void Draw(const GLuint &vao, const glm::mat4 &ViewProjectionMatrix);

private:
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();

};
#endif