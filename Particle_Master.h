#ifndef __PARTICLE_MASTER_H
#define __PARTICLE_MASTER_H

#include "Particles_V2.h"
#include "Render_Particles_V2.h"

#include <list>

class Particle_Master {
public:
	Particle_Master();
	~Particle_Master();

	void UpdateEyePosition(const glm::vec3 &eyePosition);
	void SortParticles(const glm::mat4 &V);
	void Initiate();
	std::list<Particles_V2>::iterator GenParticle(const Particle_Property &property);
	void Update();
	void Draw(const glm::mat4 &MVP);

	Render_Particles_V2 render_particles;
	std::list<Particles_V2> particles;
	std::vector<Texture_Particle> textures;

private:

};
#endif