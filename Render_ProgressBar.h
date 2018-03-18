#ifndef __RENDER_PROGRESS_BAR_H
#define __RENDER_PROGRESS_BAR_H

#include "Render.h"

class Render_ProgressBar : public Render{
public:
	Render_ProgressBar();
	~Render_ProgressBar();

	void ActivateProgram(int index);
	void SetVP(const glm::mat4 &ViewProjectionMatrix);
	void SetTexture(const GLuint &tbo, const glm::vec4 &color);
	void SetProgress(const float &progress);
	void SetOriginX(const float &originX);
	void SetSizeX(const float &sizeX);
	void SetMarker(const int &marker);
	void Highlight(const bool &highlight);
	void Draw(const GLuint &vaoNum);

private:
	void GenShaderProgram();
	void GetShaderVar();

};
#endif