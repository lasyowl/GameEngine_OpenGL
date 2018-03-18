#include <vector>
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Render_Physics_CS.h"
#include "Parse_Shader.h"
#include "Var_Render.h"
#include "GlobalFunctions.h"

#define PRIM_RESTART 0xffffff

using namespace glm;
using namespace std;

Render_Physics_CS::Render_Physics_CS() :
	width(800), height(600), clothVao(0), numElements(0),
	nParticles(20, 20), clothSize(4.0f, 4.0f),
	time(0.0f), deltaT(0.0f), speed(200.0f),
	readBuf(0), flag(false)
{
}

Render_Physics_CS::~Render_Physics_CS() {

}
int dodo = 0;
void Render_Physics_CS::initScene()
{
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(PRIM_RESTART);

	GenShaderProgram();
	initBuffers();

	projection = glm::perspective(glm::radians(50.0f), (float)width / height, 1.0f, 100.0f);

	GetShaderVar();

	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	//LoadTexture("Texture/me_textile.jpg");
	LoadTexture("Res/Texture/checker_tex.jpg");
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Render_Physics_CS::initBuffers()
{
	// Initial transform
	glm::mat4 transf = glm::translate(glm::mat4(1.0), glm::vec3(0, clothSize.y, 0));
	transf = glm::rotate(transf, glm::radians(-80.0f), glm::vec3(1, 0, 0));
	transf = glm::translate(transf, glm::vec3(0, -clothSize.y, 0));

	// Initial positions of the particles
	vector<GLfloat> initPos;
	vector<GLfloat> initVel(nParticles.x * nParticles.y * 4, 0.0f);
	vector<float> initTc;
	float dx = clothSize.x / (nParticles.x - 1);
	float dy = clothSize.y / (nParticles.y - 1);
	float ds = 1.0f / (nParticles.x - 1);
	float dt = 1.0f / (nParticles.y - 1);
	glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < nParticles.y; i++) {
		for (int j = 0; j < nParticles.x; j++) {
			p.x = dx * j;
			p.y = dy * i;
			p.z = 0.0f;
			p = transf * p;
			initPos.push_back(p.x);
			initPos.push_back(p.y);
			initPos.push_back(p.z);
			initPos.push_back(1.0f);

			initTc.push_back(ds * j);
			initTc.push_back(dt * i);
		}
	}

	// Every row is one triangle strip
	vector<GLuint> el;
	for (int row = 0; row < nParticles.y - 1; row++) {
		for (int col = 0; col < nParticles.x; col++) {
			el.push_back((row + 1) * nParticles.x + (col));
			el.push_back((row)* nParticles.x + (col));
		}
		el.push_back(PRIM_RESTART);
	}

	// We need buffers for position (2), element index,
	// velocity (2), normal, and texture coordinates.
	GLuint bufs[7];
	glGenBuffers(7, bufs);
	posBufs[0] = bufs[0];
	posBufs[1] = bufs[1];
	velBufs[0] = bufs[2];
	velBufs[1] = bufs[3];
	normBuf = bufs[4];
	elBuf = bufs[5];
	tcBuf = bufs[6];

	GLuint parts = nParticles.x * nParticles.y;

	// The buffers for positions
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBufs[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), &initPos[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, posBufs[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	// Velocities
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velBufs[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), &initVel[0], GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, velBufs[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);

	// Normal buffer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, normBuf);
	glBufferData(GL_SHADER_STORAGE_BUFFER, parts * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);

	// Element indicies
	glBindBuffer(GL_ARRAY_BUFFER, elBuf);
	glBufferData(GL_ARRAY_BUFFER, el.size() * sizeof(GLuint), &el[0], GL_DYNAMIC_COPY);

	// Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
	glBufferData(GL_ARRAY_BUFFER, initTc.size() * sizeof(GLfloat), &initTc[0], GL_STATIC_DRAW);

	numElements = GLuint(el.size());

	// Set up the VAO
	glGenVertexArrays(1, &clothVao);
	glBindVertexArray(clothVao);

	glBindBuffer(GL_ARRAY_BUFFER, posBufs[0]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normBuf);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elBuf);
	glBindVertexArray(0);
}

void Render_Physics_CS::update(float t)
{
	if (time == 0.0f) {
		deltaT = 0.0f;
	}
	else {
		deltaT = t - time;
	}
	time = t;

}

void Render_Physics_CS::SwitchFlag() {
	flag = !flag;
	if (flag == true)
		printf("Shared memory mode\n");
	else printf("Non-shared memory mode\n");
}

float dd = 5;
int dkdk = 0;
void Render_Physics_CS::render()
{
	GlobalFunctions gf;
	glFinish();
	gf.Start();
	if(flag == true)
		glUseProgram(shaderProgram[3]);
	else glUseProgram(shaderProgram[1]);

	for (int i = 0; i < 1000; i++) {
		glDispatchCompute(nParticles.x / 10, nParticles.y / 10, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Swap buffers
		readBuf = 1 - readBuf;
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBufs[readBuf]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, posBufs[1 - readBuf]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velBufs[readBuf]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, velBufs[1 - readBuf]);
	}
	glFinish();
	//gf.End("");
	dodo += gf.End();

	if (dodo < 1000)
		dkdk++;
	else {
		printf("%f msec per render\n", 1000.0f / dkdk);
		dkdk = 0;
		dodo = 0;
	}

	// Compute the normals
	glUseProgram(shaderProgram[2]);
	glDispatchCompute(nParticles.x / 10, nParticles.y / 10, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// Now draw the scene
	glUseProgram(shaderProgram[0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float color[3] = { 0.3f, 0.3f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);
	view = glm::lookAt(glm::vec3(3, 2, 6), glm::vec3(2, 1.6f, 0), glm::vec3(0, 1, 0));
	model = translate(mat4(1.0f), vec3(-2.0f, -4.0f, 0.0f));
	model = mat4(1.0f);
	setMatrices();

	// Draw the cloth
	glBindVertexArray(clothVao);
	glDrawElements(GL_TRIANGLE_STRIP, numElements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Render_Physics_CS::setMatrices() {
	glUseProgram(shaderProgram[0]);
	glm::mat4 mv = view * model;
	glm::mat3 norm = mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));

	GLuint loc_ModelViewMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewMatrix");
	GLuint loc_NormalMatrix = glGetUniformLocation(shaderProgram[0], "NormalMatrix");
	GLuint loc_MVP = glGetUniformLocation(shaderProgram[0], "MVP");
	GLuint loc_Tex = glGetUniformLocation(shaderProgram[0], "Tex");
	glUniformMatrix4fv(loc_ModelViewMatrix, 1, GL_FALSE, &mv[0][0]);
	glUniformMatrix3fv(loc_NormalMatrix, 1, GL_FALSE, &norm[0][0]);
	glUniformMatrix4fv(loc_MVP, 1, GL_FALSE, &(projection * mv)[0][0]);
	glUniform1i(loc_Tex, 0);
}

void Render_Physics_CS::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

void Render_Physics_CS::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/ads.vs" },
		{ GL_FRAGMENT_SHADER, "Shader/ads.fs" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};
	ShaderInfo shaderInfo_compute[MAX_SHADER_NUMBER] = {
		{ GL_COMPUTE_SHADER, "Shader/cloth.cs" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};
	ShaderInfo shaderInfo_compute_normal[MAX_SHADER_NUMBER] = {
		{ GL_COMPUTE_SHADER, "Shader/cloth_normal.cs" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};
	ShaderInfo shaderInfo_compute_shared[MAX_SHADER_NUMBER] = {
		{ GL_COMPUTE_SHADER, "Shader/cloth_shared.cs" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_compute));
	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_compute_normal));
	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_compute_shared));
}

void Render_Physics_CS::GetShaderVar() {
	glUseProgram(shaderProgram[0]);

	GLuint loc_LightPosition = glGetUniformLocation(shaderProgram[0], "LightPosition");
	GLuint loc_LightIntensity = glGetUniformLocation(shaderProgram[0], "LightIntensity");
	GLuint loc_Kd = glGetUniformLocation(shaderProgram[0], "Kd");
	GLuint loc_Ka = glGetUniformLocation(shaderProgram[0], "Ka");
	GLuint loc_Ks = glGetUniformLocation(shaderProgram[0], "Ks");
	GLuint loc_Shininess = glGetUniformLocation(shaderProgram[0], "Shininess");
	glUniform4fv(loc_LightPosition, 1, &(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]));
	glUniform3fv(loc_LightIntensity, 1, &(glm::vec3(1.0f)[0]));
	glUniform3fv(loc_Kd, 1, &(glm::vec3(0.8f)[0]));
	glUniform3fv(loc_Ka, 1, &(glm::vec3(0.2f)[0]));
	glUniform3fv(loc_Ks, 1, &(glm::vec3(0.2f)[0]));
	glUniform1f(loc_Shininess, 80.0f);

	glUseProgram(shaderProgram[1]);
	
	float dx = clothSize.x / (nParticles.x - 1);
	float dy = clothSize.y / (nParticles.y - 1);

	GLuint loc_RestLengthHoriz = glGetUniformLocation(shaderProgram[1], "RestLengthHoriz");
	GLuint loc_RestLengthVert = glGetUniformLocation(shaderProgram[1], "RestLengthVert");
	GLuint loc_RestLengthDiag = glGetUniformLocation(shaderProgram[1], "RestLengthDiag");
	glUniform1f(loc_RestLengthHoriz, dx);
	glUniform1f(loc_RestLengthVert, dy);
	glUniform1f(loc_RestLengthDiag, sqrtf(dx * dx + dy * dy));

	glUseProgram(shaderProgram[3]);

	glUniform1f(loc_RestLengthHoriz, dx);
	glUniform1f(loc_RestLengthVert, dy);
	glUniform1f(loc_RestLengthDiag, sqrtf(dx * dx + dy * dy));


}
