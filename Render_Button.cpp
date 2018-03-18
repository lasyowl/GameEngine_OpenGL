#include "Render_Button.h"
#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace glm;

Render_Button::Render_Button() {
	GenShaderProgram();
}

Render_Button::~Render_Button() {

}

void Render_Button::GenShaderProgram() {
	ShaderInfo shaderInfo[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Button.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Button.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo));

	GetShaderVar();
}

void Render_Button::ActivateProgram(int index) {
	glUseProgram(shaderProgram[index]);
}

void Render_Button::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ViewProjectionMatrix");
	varRender->loc_tex_background = glGetUniformLocation(shaderProgram[0], "tex_background");
	varRender->loc_tex_backgroundPack = glGetUniformLocation(shaderProgram[0], "tex_background_a");
	varRender->loc_color_background = glGetUniformLocation(shaderProgram[0], "color_background");
	varRender->loc_tex_border = glGetUniformLocation(shaderProgram[0], "tex_border");
	varRender->loc_useBorder = glGetUniformLocation(shaderProgram[0], "useBorder");
	varRender->loc_highlight = glGetUniformLocation(shaderProgram[0], "highlight");
	varRender->loc_useTexture = glGetUniformLocation(shaderProgram[0], "useTexture");
	varRender->loc_intensity = glGetUniformLocation(shaderProgram[0], "intensity");
	varRender->loc_tex_depth = glGetUniformLocation(shaderProgram[0], "texPackNum");
}

void Render_Button::SetVP(const mat4 &ViewProjectionMatrix) {
	varRender->ViewProjectionMatrix = ViewProjectionMatrix;
	glUniformMatrix4fv(varRender->loc_ViewProjectionMatrix, 1, GL_FALSE, &varRender->ViewProjectionMatrix[0][0]);
}

void Render_Button::Draw(const GLuint &vaoNum) {
	glBindVertexArray(vaoNum);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Render_Button::SetIntensity(const float &intensity) {
	glUniform1f(varRender->loc_intensity, intensity);
}

void Render_Button::SetBackground(const GLuint &tbo_background, const vec4 &color) {
	if (tbo_background == -1) {
		glUniform1i(varRender->loc_useTexture, false);
	}
	else {
		glUniform1i(varRender->loc_tex_background, tbo_background);
		glUniform1i(varRender->loc_tex_backgroundPack, tbo_background);
		glUniform1i(varRender->loc_useTexture, true);
	}
	glUniform4fv(varRender->loc_color_background, 1, &color[0]);
}

void Render_Button::SetBorder(const GLuint &tbo_border) {
	if (tbo_border == -1) {
		glUniform1i(varRender->loc_useBorder, false);
		return;
	}
	
	glUniform1i(varRender->loc_useBorder, true);
	glUniform1i(varRender->loc_tex_border, tbo_border);
}

void Render_Button::SetTexPackNum(const int &texPackNum) {
	glUniform1i(varRender->loc_tex_depth, texPackNum);
}

void Render_Button::Highlight(const bool &highlight) {
	glUniform1i(varRender->loc_highlight, highlight);
}