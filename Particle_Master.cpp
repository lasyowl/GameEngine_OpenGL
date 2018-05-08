#include "Particle_Master.h"

#include <glm/gtx/transform.hpp>
#include <algorithm>

using namespace std;
using namespace glm;

Particle_Master::Particle_Master() {

}

Particle_Master::~Particle_Master() {

}

void Particle_Master::UpdateEyePosition(const vec3 &eyePosition) {
	render_particles.eyePosition = eyePosition;
}

void Particle_Master::SortParticles(const mat4 &V) {
	auto particleIter = particles.begin();
	for (; particleIter != particles.end(); ++particleIter) {
		if (particleIter->position.size() > 1) {
			if ((V * vec4(particleIter->position.front(), 1)).z < (V * vec4(particleIter->position.back(), 1)).z) {
				std::reverse(particleIter->position.begin(), particleIter->position.end());
				std::reverse(particleIter->timeRemain.begin(), particleIter->timeRemain.end());
				particleIter->reversed = !particleIter->reversed;
			}
		}
	}
}

void Particle_Master::Initiate() {
	textures.resize(6);
	textures[0].Initiate("Res/Texture/Particle/exp2_0.png", 4, 4);
	textures[1].Initiate("Res/Texture/Particle/explosionFull.png", 4, 8);
	textures[2].Initiate("Res/Texture/Particle/Wave_01.png", 4, 8);
	textures[3].Initiate("Res/Texture/Particle/Implode_01.png", 8, 8);
	textures[4].Initiate("Res/Texture/Particle/Stars_Burst_03.png", 4, 8);
	textures[5].Initiate("Res/Texture/Particle/FlameParticleSheet.tif", 6, 6);
	render_particles.Initiate();
}

list<Particles_V2>::iterator Particle_Master::GenParticle(const Particle_Property &property) {
	particles.emplace_back(&textures[property.effectIndex], property);
	particles.back().Initiate();
	return --particles.end();
}

void Particle_Master::Update() {
	auto particleIter = particles.begin();
	for (; particleIter != particles.end(); ++particleIter) {
		if (particleIter->terminate == true) {
			particles.erase(particleIter);
			if (particleIter != particles.begin()) {
				--particleIter;
			}
		}
		else {
			particleIter->Update();
		}
	}
}

void Particle_Master::Draw(const mat4 &MVP) {
	auto particleIter = particles.begin();
	for (; particleIter != particles.end(); ++particleIter) {
		render_particles.spriteNum += (static_cast<float>(GlobalFunctions::timeElapsed) / particleIter->initProperty.cycleTime) * particleIter->texture->numSprites;
		while (render_particles.spriteNum > particleIter->texture->numSprites) {
			render_particles.spriteNum -= particleIter->texture->numSprites;
		}
		render_particles.intensity = particleIter->initProperty.initTimeRemain * 0.002f;
		render_particles.loop = particleIter->initProperty.loop;
		render_particles.cycleTime = particleIter->initProperty.cycleTime;
		render_particles.width = particleIter->texture->numCol;
		render_particles.height = particleIter->texture->numRow;
		render_particles.Draw(MVP, particleIter->vao, particleIter->position.size(), particleIter->texture->tbo);
	}
}