#include "Render_Postprocess.h"
#include "Var_Render.h"
#include "Parse_Shader.h"

using namespace glm;

Render_Postprocess::Render_Postprocess() : currentShader(0) {
	
}

Render_Postprocess::~Render_Postprocess() {

}

void Render_Postprocess::Initiate() {
	GenShaderProgram();
	GetShaderVar();
}

void Render_Postprocess::SetShader(const int &program) {
	currentShader = program;
	glUseProgram(shaderProgram[program]);
}

void Render_Postprocess::SetVP(const mat4 &VP) {
	varRender->ModelViewProjectionMatrix = VP;
}

void Render_Postprocess::SetVAO(const GLuint &vao) {
	this->vao = vao;
}

void Render_Postprocess::SetIntensity(const float &intensity) {
	varRender->intensity = intensity;
}

void Render_Postprocess::SetTexture(const GLuint &texture_fbo) {
	tex_framebuffer = texture_fbo;
}

void Render_Postprocess::SetTexture_Ext(const GLuint &texture_ext, const int &target) {
	switch (target) {
	case PP_TEX_GBLUR:
		glUseProgram(shaderProgram[SHADER_PP_GBLUR]);
		glUniform1i(varRender_GBlur->loc_tex_gblur, texture_ext);
		break;
	default:
		break;
	}
}

void Render_Postprocess::SetShaderState(const int &state) {
	switch (currentShader) {
	case SHADER_PP_GBLUR:
		glUniform1i(varRender_GBlur->loc_state_gblur, state);
		break;
	default:
		break;
	}
}

void Render_Postprocess::PrepareDraw() {
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	SetShaderVar();
	glBindVertexArray(vao);
}

void Render_Postprocess::Draw() {
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Render_Postprocess::FinishDraw() {
	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void Render_Postprocess::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Postprocess_Default.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Postprocess_Default.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};
	ShaderInfo shaderInfo_gBlur[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Postprocess_GBlur.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Postprocess_GBlur.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_gBlur));
}

void Render_Postprocess::GetShaderVar() {
	glUseProgram(shaderProgram[SHADER_PP_DEFAULT]);
	varRender = new Var_Render;
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[SHADER_PP_DEFAULT], "ModelViewProjectionMatrix");
	varRender->loc_tex_framebuffer = glGetUniformLocation(shaderProgram[SHADER_PP_DEFAULT], "tex_framebuffer");
	varRender->loc_intensity = glGetUniformLocation(shaderProgram[SHADER_PP_DEFAULT], "intensity");

	glUseProgram(shaderProgram[SHADER_PP_GBLUR]);
	varRender_GBlur = new Var_Render;
	varRender_GBlur->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[SHADER_PP_GBLUR], "ModelViewProjectionMatrix");
	varRender_GBlur->loc_tex_framebuffer = glGetUniformLocation(shaderProgram[SHADER_PP_GBLUR], "tex_framebuffer");
	varRender_GBlur->loc_tex_gblur = glGetUniformLocation(shaderProgram[SHADER_PP_GBLUR], "tex_gblur");
	varRender_GBlur->loc_intensity = glGetUniformLocation(shaderProgram[SHADER_PP_GBLUR], "intensity");
	varRender_GBlur->loc_state_gblur = glGetUniformLocation(shaderProgram[SHADER_PP_GBLUR], "state_gblur");
}

void Render_Postprocess::SetShaderVar() {
	switch (currentShader) {
	case SHADER_PP_DEFAULT:
		glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
		glUniform1i(varRender->loc_tex_framebuffer, tex_framebuffer);
		glUniform1f(varRender->loc_intensity, varRender->intensity);
		break;
	case SHADER_PP_GBLUR:
		glUniformMatrix4fv(varRender_GBlur->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
		glUniform1i(varRender_GBlur->loc_tex_framebuffer, tex_framebuffer);
		glUniform1f(varRender_GBlur->loc_intensity, varRender->intensity);
		break;
	default:
		break;
	}
}