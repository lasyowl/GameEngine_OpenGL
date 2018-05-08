#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Render_Sphere.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace std;
using namespace glm;

Render_Sphere::Render_Sphere() : highlight(false), radius(1.0f) {

}

Render_Sphere::~Render_Sphere() {

}

void Render_Sphere::Initiate() {
	GenShaderProgram();
	GetShaderVar();
}

void Render_Sphere::SetRadius(const float &radius) {
	this->radius = radius;
}

void Render_Sphere::Draw(const mat4 &ViewProjectionMatrix, const GLuint &vao, const int &indexCount) {
	varRender->ModelViewProjectionMatrix = ViewProjectionMatrix;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(shaderProgram[0]);
	glBindVertexArray(vao);

	SetShaderVar();

	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, (GLvoid*)0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Render_Sphere::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Sphere.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Sphere.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
}

void Render_Sphere::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_highlight = glGetUniformLocation(shaderProgram[0], "highlight");
	varRender->loc_pos_collider = glGetUniformLocation(shaderProgram[0], "pos_collider");
	varRender->loc_radius = glGetUniformLocation(shaderProgram[0], "radius");
}

void Render_Sphere::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
	glUniform1i(varRender->loc_highlight, highlight);
	glUniform1f(varRender->loc_radius, radius);
}