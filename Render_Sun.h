#ifndef __RENDER_SUN_H
#define __RENDER_SUN_H

#include "Render.h"

class Sun;

class Render_Sun : public Render {
public:
	Render_Sun();
	~Render_Sun();

	void Draw(const glm::mat4 &ViewProjectionMatrix);
	void SetData(Sun *sun);
	void UpdateAngle(const glm::vec2 &angle);
	void Initiate();

	bool drawFlare;

private:
	void GenShaderProgram();
	void GetShaderVar();
	void UpdateShaderVar();

	Sun *sun;
	glm::vec2 angle;
};
#endif