#include "Render_Line.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace glm;

Render_Line::Render_Line() {

}

Render_Line::~Render_Line() {

}

void Render_Line::Initiate() {
	GenShaderProgram();
	GetShaderVar();
}

void Render_Line::Draw(const GLuint &vao, const mat4 &ViewProjectionMatrix) {
	varRender->ViewProjectionMatrix = ViewProjectionMatrix;

	glUseProgram(shaderProgram[0]);
	SetShaderVar();
	glBindVertexArray(vao);

	glDrawArrays(GL_LINES, 0, 2);
}

void Render_Line::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Line.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Line.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
}

void Render_Line::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
}

void Render_Line::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
}