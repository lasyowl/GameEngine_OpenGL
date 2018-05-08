#include "Render_HUD.h"
#include "Parse_Shader.h"

using namespace glm;

Render_HUD::Render_HUD() {

}

Render_HUD::~Render_HUD() {

}

void Render_HUD::Initiate() {
	GenShaderProgram();
}

void Render_HUD::UpdateCameraPos(const glm::vec3 &position) {
	glUseProgram(shaderProgram[0]);
	glUniform3fv(varRender->loc_eye_position, 1, &position.x);
}

void Render_HUD::UpdateVP(const mat4 &VP) {
	glUseProgram(shaderProgram[0]);
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &VP[0][0]);
}

void Render_HUD::Draw(const glm::vec3 &position, const float &portion) {
	this->position = position;
	this->portion = portion;

	glUseProgram(shaderProgram[0]);
	UpdateShaderVar();

	glDrawArrays(GL_POINTS, 0, 1);
}

void Render_HUD::GenShaderProgram() {
	ShaderInfo shaderInfo[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/GUI_HUD.vert" },
		{ GL_GEOMETRY_SHADER, "Shader/GUI_HUD.geom" },
		{ GL_FRAGMENT_SHADER, "Shader/GUI_HUD.frag" },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo));

	GetShaderVar();
}

void Render_HUD::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_eye_position = glGetUniformLocation(shaderProgram[0], "eyePosition");
	varRender->loc_position = glGetUniformLocation(shaderProgram[0], "position");
	varRender->loc_portion = glGetUniformLocation(shaderProgram[0], "portion");
}

void Render_HUD::UpdateShaderVar() {
	glUniform3fv(varRender->loc_position, 1, &position.x);
	glUniform1f(varRender->loc_portion, portion);
}