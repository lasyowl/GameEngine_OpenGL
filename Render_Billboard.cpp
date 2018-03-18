#include "Render_Billboard.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace std;
using namespace glm;

Render_Billboard::Render_Billboard() {

}

Render_Billboard::~Render_Billboard() {

}

void Render_Billboard::GenShaderProgram() {
	ShaderInfo shaderInfo[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Billboard.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Billboard.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo));

	GetShaderVar();
}

void Render_Billboard::ActivateProgram(const int &index) {
	glUseProgram(shaderProgram[index]);
}

void Render_Billboard::SetTexture(const GLuint &tbo) {
	glUniform1i(varRender->loc_tex_background, tbo);
}

void Render_Billboard::SetMVP(const mat4 &MVP) {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
}

void Render_Billboard::Draw_Grass(const GLuint &vaoNum) {
	glBindVertexArray(vaoNum);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Render_Billboard::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_tex_background = glGetUniformLocation(shaderProgram[0], "tex_background");
}