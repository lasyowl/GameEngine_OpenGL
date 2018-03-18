#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Render_Terrain.h"
#include "Parse_Shader.h"
#include "Light.h"
#include "Framebuffer.h"
#include "Var_Render.h"
#include "Var_Compute.h"
#include "GlobalFunctions.h"

using namespace std;
using namespace glm;

Render_Terrain::Render_Terrain() {

}

Render_Terrain::~Render_Terrain() {

}

void Render_Terrain::Initiate() {
	GenShaderProgram();
	GetShaderVar();
}

void Render_Terrain::SetTerrainWidth(const int &width) {
	terrainWidth = width;
}

void Render_Terrain::SetVP(const mat4 &VP) {
	varRender->ModelViewProjectionMatrix = VP;
}

void Render_Terrain::SetVAO(const GLuint &vao) {
	this->vao = vao;
}

void Render_Terrain::SetIBO(const GLuint &ibo, const int &numIndex) {
	this->ibo = ibo;
	this->numIndex = numIndex;
}

void Render_Terrain::SetLight(Light *light) {
	this->light = light;
}

void Render_Terrain::SetFramebuffer(Framebuffer *framebuffer) {
	this->framebuffer = framebuffer;
}

void Render_Terrain::SetTexture_Terrain(Texture_Terrain *texture_terrain) {
	this->texture_terrain = texture_terrain;
}

void Render_Terrain::SetShadowMatrix(const mat4 &ShadowMatrix) {
	varRender->ShadowMatrix = ShadowMatrix;
}

void Render_Terrain::UpdateEyePos(const vec3 &eyePosition) {
	varRender->eyePosition = eyePosition;
}

void Render_Terrain::EnableMode(const int &mode) {
	this->mode[mode] = true;
}

void Render_Terrain::DisableMode(const int &mode) {
	this->mode[mode] = false;
}


void Render_Terrain::PrepareDraw() {
	glFrontFace(GL_CW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glUseProgram(shaderProgram[0]);
	SetShaderVar();
	glBindVertexArray(vao);
}

void Render_Terrain::Draw() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLE_STRIP, numIndex, GL_UNSIGNED_INT, (GLvoid*)0);
}

void Render_Terrain::FinishDraw() {
	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFrontFace(GL_CCW);
}

void Render_Terrain::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Terrain.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Terrain.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
}

void Render_Terrain::GetShaderVar() {
	glUseProgram(shaderProgram[0]);
	varRender = new Var_Render;
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_numLight = glGetUniformLocation(shaderProgram[0], "numLight");
	varRender->loc_light_intensity = glGetUniformLocation(shaderProgram[0], "light_intensity");
	varRender->loc_light_position = glGetUniformLocation(shaderProgram[0], "light_position");
	varRender->loc_light_direction = glGetUniformLocation(shaderProgram[0], "light_direction");
	varRender->loc_color_ambient = glGetUniformLocation(shaderProgram[0], "color_ambient");
	varRender->loc_color_diffuse = glGetUniformLocation(shaderProgram[0], "color_diffuse");
	varRender->loc_color_specular = glGetUniformLocation(shaderProgram[0], "color_specular");
	varRender->loc_eye_position = glGetUniformLocation(shaderProgram[0], "eye_position");
	varRender->loc_heightmapWidthHeight = glGetUniformLocation(shaderProgram[0], "heightmapWidthHeight");
	varRender->loc_tex_terrainPack = glGetUniformLocation(shaderProgram[0], "tex_terrainPack");
	varRender->loc_tex_normalPack = glGetUniformLocation(shaderProgram[0], "tex_normalPack");
	varRender->loc_tex_specularPack = glGetUniformLocation(shaderProgram[0], "tex_specularPack");
	varRender->loc_tex_alphamap = glGetUniformLocation(shaderProgram[0], "tex_alphamap");
	varRender->loc_tex_shadowmap = glGetUniformLocation(shaderProgram[0], "tex_shadowmap");
	varRender->loc_shadowMatrix = glGetUniformLocation(shaderProgram[0], "ShadowMatrix");
	varRender->loc_fastMode = glGetUniformLocation(shaderProgram[0], "fastMode");
	glUniform1f(varRender->loc_heightmapWidthHeight, terrainWidth);
	glUniform1i(varRender->loc_tex_terrainPack, texture_terrain->texture_terrainPack);
	glUniform1i(varRender->loc_tex_normalPack, texture_terrain->texture_normalPack);
	glUniform1i(varRender->loc_tex_specularPack, texture_terrain->texture_specularPack);
	glUniform1i(varRender->loc_tex_alphamap, texture_terrain->texture_alphamap);
	glUniform1i(varRender->loc_tex_shadowmap, framebuffer->depthTex_shadowmap);
	glUniform1i(varRender->loc_fastMode, mode[FAST_MODE]);
}

void Render_Terrain::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(varRender->loc_shadowMatrix, 1, GL_FALSE, &varRender->ShadowMatrix[0][0]);
	glUniform1i(varRender->loc_numLight, light->numLight);
	glUniform1fv(varRender->loc_light_intensity, light->numLight, &light->intensity[0]);
	glUniform3fv(varRender->loc_light_position, light->numLight, &light->position[0][0]);
	glUniform3fv(varRender->loc_light_direction, light->numLight, &light->direction[0][0]);
	glUniform3fv(varRender->loc_eye_position, 1, &varRender->eyePosition[0]);
	glUniform1i(varRender->loc_fastMode, mode[FAST_MODE]);
}