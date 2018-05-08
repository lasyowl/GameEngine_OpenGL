#include "Render_Sun.h"
#include "Sun.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace std;
using namespace glm;

Render_Sun::Render_Sun() : sun(nullptr), drawFlare(true) {

}

Render_Sun::~Render_Sun() {

}

void Render_Sun::UpdateAngle(const vec2 &angle) {
	this->angle = angle;
}

void Render_Sun::Draw(const mat4 &ViewProjectionMatrix) {
	varRender->ModelViewProjectionMatrix = ViewProjectionMatrix;
	glUseProgram(shaderProgram[0]);
	UpdateShaderVar();
	glBindVertexArray(sun->vao);
	glUniform1i(varRender->loc_sun_seq, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (drawFlare == false)
		return;
	for (int i = 1; i < 10; i++) {
		glUniform1i(varRender->loc_sun_seq, i);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

void Render_Sun::Initiate() {
	GenShaderProgram();
	GetShaderVar();
}

void Render_Sun::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Sun.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Sun.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
}

void Render_Sun::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_tex_core = glGetUniformLocation(shaderProgram[0], "tex_core");
	varRender->loc_tex_lens = glGetUniformLocation(shaderProgram[0], "tex_lens");
	varRender->loc_sun_seq = glGetUniformLocation(shaderProgram[0], "sequence");
	varRender->loc_angle = glGetUniformLocation(shaderProgram[0], "angle");
}

void Render_Sun::UpdateShaderVar() {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
	glUniform1i(varRender->loc_tex_core, sun->tex_core);
	glUniform1iv(varRender->loc_tex_lens, 9, (GLint*)sun->tex_lens);
	glUniform2fv(varRender->loc_angle, 1, &angle[0]);
}

void Render_Sun::SetData(Sun *sun) {
	this->sun = sun;
}