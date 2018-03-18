#include "Render_ProgressBar.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

Render_ProgressBar::Render_ProgressBar() {
	GenShaderProgram();
}

Render_ProgressBar::~Render_ProgressBar() {

}

void Render_ProgressBar::ActivateProgram(int index) {
	glUseProgram(shaderProgram[index]);
}

void Render_ProgressBar::SetVP(const glm::mat4 &ViewProjectionMatrix) {
	varRender->ViewProjectionMatrix = ViewProjectionMatrix;
	glUniformMatrix4fv(varRender->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
}

void Render_ProgressBar::SetProgress(const float &progress) {
	glUniform1f(varRender->loc_progress, progress);
}

void Render_ProgressBar::SetOriginX(const float &originX) {
	glUniform1f(varRender->loc_originX, originX);
}

void Render_ProgressBar::SetSizeX(const float &sizeX) {
	glUniform1f(varRender->loc_sizeX, sizeX);
}

void Render_ProgressBar::SetMarker(const int &marker) {
	glUniform1i(varRender->loc_marker, marker);
}

void Render_ProgressBar::SetTexture(const GLuint &tbo, const glm::vec4 &color) {
	if (tbo == -1)
		glUniform1i(varRender->loc_useTexture, false);
	else {
		glUniform1i(varRender->loc_useTexture, true);
		glUniform1i(varRender->loc_tex_background, tbo);
	}
	glUniform4fv(varRender->loc_color_background, 1, &color[0]);
}

void Render_ProgressBar::Highlight(const bool &highlight) {
	glUniform1i(varRender->loc_highlight, highlight);
}

void Render_ProgressBar::Draw(const GLuint &vaoNum) {
	glBindVertexArray(vaoNum);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Render_ProgressBar::GenShaderProgram() {
	ShaderInfo shaderInfo[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/ProgressBar.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/ProgressBar.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo));

	GetShaderVar();
}

void Render_ProgressBar::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ViewProjectionMatrix");
	varRender->loc_tex_background = glGetUniformLocation(shaderProgram[0], "tex_background");
	varRender->loc_color_background = glGetUniformLocation(shaderProgram[0], "color_background");
	varRender->loc_highlight = glGetUniformLocation(shaderProgram[0], "highlight");
	varRender->loc_useTexture = glGetUniformLocation(shaderProgram[0], "useTexture");
	varRender->loc_progress = glGetUniformLocation(shaderProgram[0], "progress");
	varRender->loc_originX = glGetUniformLocation(shaderProgram[0], "originX");
	varRender->loc_sizeX = glGetUniformLocation(shaderProgram[0], "sizeX");
	varRender->loc_marker = glGetUniformLocation(shaderProgram[0], "marker");
}
