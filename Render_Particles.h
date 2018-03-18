#ifndef __RENDER_PARTICLES_H
#define __RENDER_PARTICLES_H

#include <list>
#include "Particles.h"
#include "Render.h"

class Var_Compute;

class Render_Particles : public Render {
public:
	Render_Particles();
	~Render_Particles();

	void Draw(glm::mat4 ViewProjecitonMatrix);
	void AddParticle(int timeRemain, glm::vec3 position, glm::vec3 velocity, int particleNum);

private:
	void InitParticleRenderer();
	void GenShaderProgram();
	void GetShaderVar();
	void GenTextureObjects();
	void SetShaderVar();
	void GenObjects(Particles *particle);
	void BindObjectData(Particles *particle);
	void DispatchCS();
	
	std::list<Particles> particles;
	GLuint texture_particle;
	GLuint randTexture;

	Var_Compute *varCompute;
};
#endif