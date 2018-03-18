#ifndef __RENDER_TEXT_H
#define __RENDER_TEXT_H

#include "Render.h"

class Render_Text : public Render {
public:
	Render_Text();
	~Render_Text();

	void ActivateProgram(int index);
	void SetTextureAtlas(const GLuint &tbo_tex);
	void SetTexturePage(const GLuint &textPage);
	void SetCharOffset(const glm::vec2 &offset);
	void SetFontSize(const float &size);
	void SetBoxOffset(const glm::vec2 &offset);
	void SetVP(const glm::mat4 &ViewProjectionMatrix);
	void SetTextColor(const glm::vec4 &color);
	void Draw(const GLuint &vaoNum);

private:
	void GenShaderProgram();
	void GetShaderVar();

};
#endif