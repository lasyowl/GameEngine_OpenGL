#include "Particles_V2.h"

using namespace std;
using namespace glm;

Particles_V2::Particles_V2(Texture_Particle *texture, const Particle_Property &initProperty) : terminate(false), reversed(false) {
	this->texture = texture;
	this->initProperty = initProperty;
	// Temporary
}

Particles_V2::~Particles_V2() {
	glDeleteBuffers(VBO_NUM, vbo);
	glDeleteVertexArrays(1, &vao);
}

void Particles_V2::Initiate() {
	GenData();
	GenBuffer();
}

void Particles_V2::GenData() {
	position.resize(initProperty.maxParticleNum, initProperty.origin);
	direction.resize(initProperty.maxParticleNum);
	scale.resize(initProperty.maxParticleNum);
	velocity.resize(initProperty.maxParticleNum);
	timeRemain.resize(initProperty.maxParticleNum);

	float halfMax = RAND_MAX * 0.5f;
	float invRAND_MAX = 1.0f / RAND_MAX;

	timeRemain_global = initProperty.initTimeRemain;
	for (int i = 0; i < position.size(); i++) {
		position[i] += i * initProperty.initVelocity * initProperty.initDir;
		position[i] += invRAND_MAX * initProperty.distortion * vec3(rand() - halfMax, rand() - halfMax, rand() - halfMax);
		float timeInterval = i * (initProperty.initTimeRemain - initProperty.cycleTime) / static_cast<float>(initProperty.maxParticleNum);
		timeRemain[i] = initProperty.cycleTime + timeInterval;
	}
}

void Particles_V2::GenBuffer() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(VBO_NUM, vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(vec3), &position[0][0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_DIRECTION]);
	glBufferData(GL_ARRAY_BUFFER, direction.size() * sizeof(vec3), &direction[0][0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_SCALE]);
	glBufferData(GL_ARRAY_BUFFER, scale.size() * sizeof(float), &scale[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_VELOCITY]);
	glBufferData(GL_ARRAY_BUFFER, velocity.size() * sizeof(float), &velocity[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_TIMEREMAIN]);
	glBufferData(GL_ARRAY_BUFFER, timeRemain.size() * sizeof(float), &timeRemain[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(4);
}

void Particles_V2::Update() {
	UpdateData();
	UpdateBuffer();
	UpdateCollider();
}

void Particles_V2::UpdateData() {

	timeRemain_global -= GlobalFunctions::timeElapsed;
	if (initProperty.loop == true) {
		position[0] = origin;
		if (timeRemain_global <= 0) {
			this->terminate = true;
		}
		if (timeRemain[0] <= 0) {
			timeRemain[0] += initProperty.cycleTime;
		}
	}

	for (int i = 0; i < position.size(); i++) {
		timeRemain[i] -= GlobalFunctions::timeElapsed;
		if (initProperty.loop == false) {
			if (reversed == false) {
				if (timeRemain.back() <= 0) {
					this->terminate = true;
				}
			}
			else {
				if (timeRemain.front() <= 0) {
					this->terminate = true;
				}
			}
		}
	}
}

void Particles_V2::UpdateBuffer() {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(vec3), &position[0].x, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_DIRECTION]);
	glBufferData(GL_ARRAY_BUFFER, direction.size() * sizeof(vec3), &direction[0].x, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_SCALE]);
	glBufferData(GL_ARRAY_BUFFER, scale.size() * sizeof(float), &scale[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_VELOCITY]);
	glBufferData(GL_ARRAY_BUFFER, velocity.size() * sizeof(float), &velocity[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_TIMEREMAIN]);
	glBufferData(GL_ARRAY_BUFFER, timeRemain.size() * sizeof(float), &timeRemain[0], GL_DYNAMIC_DRAW);
}

void Particles_V2::UpdateCollider() {
	float timeRemain_ratio;
	if (reversed == false) {
		timeRemain_ratio = 1 - (timeRemain_global - initProperty.cycleTime) / (initProperty.initTimeRemain - initProperty.cycleTime);
	}
	else {
		timeRemain_ratio = (timeRemain_global - initProperty.cycleTime) / (initProperty.initTimeRemain - initProperty.cycleTime);
	}
	int left = floor(timeRemain_ratio * (position.size() - 1));
	int right = ceil(timeRemain_ratio * (position.size() - 1));
	float interpolate = fract(timeRemain_global);
	vec3 interpolatedPos = (1 - interpolate) * position[left] + interpolate * position[right];

	collider.UpdateVariables(interpolatedPos, 4);
}