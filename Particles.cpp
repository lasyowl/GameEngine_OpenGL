#include "Particles.h"

using namespace glm;

Particles::Particles() {
	Info_Particle initValue = { vec4(TYPE_SHELL), vec4(0.0f), vec4(0.0f), vec4(0.0f), vec4(0.0f) };
	info.resize(MAX_NUM_PARTICLES, initValue);
}

Particles::Particles(int timeRemain, glm::vec3 position, glm::vec3 velocity, int particleNum = 0) {
	Info_Particle initValue = { vec4(TYPE_SHELL), vec4(0.0f), vec4(0.0f), vec4(0.0f), vec4(particleNum) };
	info.resize(MAX_NUM_PARTICLES, initValue);
	InitParticles(timeRemain, position, velocity, particleNum);
}

Particles::~Particles() {

}

void Particles::InitParticles(int timeRemain, vec3 position, vec3 velocity, int particleNum) {
	info[0] = { vec4(TYPE_LAUNCHER), vec4(position, 1.0f), vec4(velocity, 1.0f), vec4(timeRemain), vec4(particleNum) };
}