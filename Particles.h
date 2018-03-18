#ifndef __PARTICLES_H
#define __PARTICLES_H

#include <vector>
#include <glm/glm.hpp>

#define MAX_NUM_PARTICLES 10
#define TYPE_LAUNCHER 0.0f
#define TYPE_SHELL 1.0f
#define TYPE_REGULAR 2.0f
#define TYPE_DESTROY 3.0f

typedef unsigned int GLuint;

struct Info_Particle {
	glm::vec4 type;
	glm::vec4 position;
	glm::vec4 velocity;
	glm::vec4 timeRemain;
	glm::vec4 particleNum;
};

class Particles {
public:
	Particles();
	Particles(int timeRemain, glm::vec3 position, glm::vec3 velocity, int particleNum);
	~Particles();

	void InitParticles(int timeRemain, glm::vec3 position, glm::vec3 velocity, int particleNum);

	GLuint vao;
	GLuint vbo_vertex;
	std::vector<Info_Particle> info;

private:
};
#endif