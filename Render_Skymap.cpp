#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Render_Skymap.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace std;
using namespace glm;

Render_Skymap::Render_Skymap() : skyMode(false) {
	varRender = new Var_Render;
	this->skyMode = skyMode;
}

Render_Skymap::~Render_Skymap() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo_vertex);
	glDeleteBuffers(1, &vbo_normal);
	glDeleteBuffers(1, &vbo_uv);
	glDeleteBuffers(1, &index);
	glDeleteTextures(1, &texture);
	delete varRender;
}

void Render_Skymap::Initiate() {
	GenSphereMeshes();
	InitSphereRenderer();
	GenShaderProgram();
	GetShaderVar();
}

void Render_Skymap::InitSphereRenderer() {
	GenObjects();
	BindObjectData();
}

void Render_Skymap::EnableSkyMode() {
	this->skyMode = true;
}

void Render_Skymap::DisableSkyMode() {
	this->skyMode = false;
}

// Terrain mesh generation without using LOD.
void Render_Skymap::GenSphereMeshes() {

	sphereMesh.InitMesh(50);
}

void Render_Skymap::GenObjects() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_normal);
	glGenBuffers(1, &vbo_uv);
	glGenBuffers(1, &index);
	glGenTextures(1, &texture);
}

void Render_Skymap::BindObjectData() {

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, sphereMesh.vertex.size() * 3 * sizeof(float), &sphereMesh.vertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sphereMesh.normal.size() * 3 * sizeof(float), &sphereMesh.normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sphereMesh.uv.size() * 2 * sizeof(float), &sphereMesh.uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereMesh.index.size() * sizeof(GLuint), &sphereMesh.index[0], GL_STATIC_DRAW);

	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	LoadTexture("Res/Spheremap/skymap.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Render_Skymap::Draw(mat4 ViewProjectionMatrix) {

	varRender->ModelViewProjectionMatrix = ViewProjectionMatrix;

	glEnable(GL_CULL_FACE);
	if (skyMode == true) {
		glDisable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shaderProgram[0]);
	glBindVertexArray(vao);

	SetShaderVar();

	glDrawElements(GL_TRIANGLE_STRIP, sphereMesh.index.size(), GL_UNSIGNED_INT, (GLvoid*)0);

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
}

void Render_Skymap::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Default.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Default.frag" },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
}

void Render_Skymap::GetShaderVar() {
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_texture_diffuse = glGetUniformLocation(shaderProgram[0], "texture_diffuse");
}

void Render_Skymap::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
	glUniform1i(varRender->loc_texture_diffuse, texture);
}