#ifndef __RENDER_HUD_H
#define __RENDER_HUD_H

#include "Render.h"

#include <glm/glm.hpp>

class Render_HUD : public Render {
public:
	Render_HUD();
	~Render_HUD();

	void Initiate();
	void UpdateCameraPos(const glm::vec3 &position);
	void UpdateVP(const glm::mat4 &VP);
	void Draw(const glm::vec3 &position, const float &portion);

private:
	void GenShaderProgram();
	void GetShaderVar();
	void UpdateShaderVar();

	glm::vec3 position;
	float portion;
};
#endif