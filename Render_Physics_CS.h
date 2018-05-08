#include <glm/glm.hpp>
#include "Render.h"

class Render_Physics_CS : public Render {

public:
	Render_Physics_CS();
	~Render_Physics_CS();

	void InitScene();
	void Update(float t);
	void Render();
	void Resize(int, int);
	void SwitchFlag();

private:
	void GenShaderProgram();
	void GetShaderVar();
	void InitBuffers();
	void SetMatrices();

	int width, height;
	GLuint clothVao;
	GLuint numElements;

	glm::ivec2 nParticles;  // Number of particles in each dimension
	glm::vec2 clothSize;    // Size of cloth in x and y

	float time, deltaT, speed;
	GLuint readBuf;
	GLuint posBufs[2], velBufs[2];
	GLuint normBuf, elBuf, tcBuf;

	glm::mat4 view, model, projection;
	bool flag;
	GLuint tex;
};
