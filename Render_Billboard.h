#ifndef __RENDER_BILLBOARD_H
#define __RENDER_BILLBOARD_H

#include "Render.h"

class Render_Billboard : public Render {
public:
	Render_Billboard();
	~Render_Billboard();

	void ActivateProgram(const int &index);
	void SetTexture(const GLuint &tbo);
	void SetMVP(const glm::mat4 &MVP);
	void Draw_Grass(const GLuint &vaoNum);

private:
	void GenShaderProgram();
	void GetShaderVar();
};
#endif