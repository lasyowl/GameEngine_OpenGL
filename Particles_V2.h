#ifndef __PARTICLES_V2_H
#define __PARTICLES_V2_H

#include "Texture_Particle.h"

#include "GlobalFunctions.h"
#include "Collider_Sphere.h"
#include "Character.h"

#include <glm/glm.hpp>
#include <vector>

enum Vbo_Particle { VBO_POSITION, VBO_DIRECTION, VBO_SCALE, VBO_VELOCITY, VBO_TIMEREMAIN, VBO_NUM };

struct Particle_Property {
	int effectIndex;
	int maxParticleNum;
	int numLaunchers;			// Not in use
	int interval;				// Not in use
	glm::vec3 origin;
	glm::vec3 initDir;
	float initVelocity;
	float scale;				// Not in use
	int initTimeRemain;
	int cycleTime;
	float distortion;
	bool loop;
	Character *owner;
};

class Particles_V2 {
public:
	Particles_V2(Texture_Particle *texture, const Particle_Property &initProperty);
	~Particles_V2();

	void Initiate();
	void Update();

	Texture_Particle *texture;
	Particle_Property initProperty;
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> direction;
	std::vector<float> scale;
	std::vector<float> velocity;
	std::vector<float> timeRemain;
	float timeRemain_global;
	bool terminate;
	bool reversed;
	GLuint vao;
	GLuint vbo[VBO_NUM];
	Particles_V2 *successor;
	Collider_Sphere collider;
	glm::vec3 origin;

private:
	void GenData();
	void GenBuffer();
	void UpdateData();
	void UpdateBuffer();
	void UpdateCollider();
};
#endif