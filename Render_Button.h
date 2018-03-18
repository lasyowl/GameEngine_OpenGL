#ifndef __RENDER_BUTTON_H
#define __RENDER_BUTTON_H

#include "Render.h"

class Render_Button : public Render {
public:
	Render_Button();
	~Render_Button();

	void ActivateProgram(int index);
	void SetVP(const glm::mat4 &ViewProjectionMatrix);
	void SetIntensity(const float &intensity);
	void SetBackground(const GLuint &tbo_background, const glm::vec4 &color);
	void SetBorder(const GLuint &tbo_boarder);
	void SetTexPackNum(const int &texPackNum);
	void Highlight(const bool &isUnderMouse);
	void Draw(const GLuint &vaoNum);

private:
	void GenShaderProgram();
	void GetShaderVar();

};
#endif