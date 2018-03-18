#include "Render_Text.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace glm;

Render_Text::Render_Text() {
	GenShaderProgram();
}

Render_Text::~Render_Text() {

}

void Render_Text::GenShaderProgram() {
	ShaderInfo shaderInfo[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Text.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Text.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo));
	
	GetShaderVar();
}

void Render_Text::ActivateProgram(int index) {
	glUseProgram(shaderProgram[index]);
}

void Render_Text::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ViewProjectionMatrix");
	varRender->loc_tex_text = glGetUniformLocation(shaderProgram[0], "tex_text");
	varRender->loc_color_text = glGetUniformLocation(shaderProgram[0], "color_text");
	varRender->loc_textPage = glGetUniformLocation(shaderProgram[0], "textPage");
	varRender->loc_textOffset = glGetUniformLocation(shaderProgram[0], "textOffset");
	varRender->loc_boxOffset = glGetUniformLocation(shaderProgram[0], "boxOffset");
	varRender->loc_fontSize = glGetUniformLocation(shaderProgram[0], "fontSize");
}

void Render_Text::SetTextureAtlas(const GLuint &tbo_tex) {
	glUniform1i(varRender->loc_tex_text, tbo_tex);
}

void Render_Text::SetTexturePage(const GLuint &textPage) {
	glUniform1i(varRender->loc_textPage, textPage);
}

void Render_Text::SetCharOffset(const vec2 &offset) {
	glUniform2fv(varRender->loc_textOffset, 1, &offset[0]);
}

void Render_Text::SetBoxOffset(const vec2 &offset) {
	glUniform2fv(varRender->loc_boxOffset, 1, &offset[0]);
}

void Render_Text::SetFontSize(const float &size) {
	glUniform1f(varRender->loc_fontSize, size);
}

void Render_Text::SetVP(const mat4 &ViewProjectionMatrix) {
	varRender->ViewProjectionMatrix = ViewProjectionMatrix;
	glUniformMatrix4fv(varRender->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
}

void Render_Text::Draw(const GLuint &vaoNum) {
	glBindVertexArray(vaoNum);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Render_Text::SetTextColor(const vec4 &color) {
	glUniform4fv(varRender->loc_color_text, 1, &color[0]);
}