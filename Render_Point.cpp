#include "Render_Point.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace glm;

Render_Point::Render_Point() {
	point = vec3(0, 0, 0);
}

Render_Point::~Render_Point() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Render_Point::Initiate() {
	GenObjects();
	BindObjects();
	GenShaderProgram();
	GetShaderVar();
}

void Render_Point::GenObjects() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}

void Render_Point::BindObjects() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3), &point[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

void Render_Point::UpdatePoint(const vec3 &point) {
	this->point = point;
}

void Render_Point::Draw(const mat4 &ViewProjectionMatrix) {
	varRender->ViewProjectionMatrix = ViewProjectionMatrix;
	
	glUseProgram(shaderProgram[0]);
	SetShaderVar();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3), &point[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//SetShaderVar();
	glPointSize(5.0f);
	glDrawArrays(GL_POINTS, 0, 10);
}

void Render_Point::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Point.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Point.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
}

void Render_Point::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ViewProjectionMatrix");
}

void Render_Point::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
}