#ifndef __RENDER_PARTICLES_V2_H
#define __RENDER_PARTICLES_V2_H

#include "Render.h"

class Render_Particles_V2 : public Render {
public:
	Render_Particles_V2();
	~Render_Particles_V2();

	void Initiate();
	void Draw(const glm::mat4 &MVP, const GLuint &vao, const int &drawCount, const GLuint &tex);

	glm::vec3 eyePosition;
	int width;
	int height;
	float spriteNum;
	float cycleTime;
	bool loop;
	float intensity;

private:
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();

	GLuint tex;
};
#endif