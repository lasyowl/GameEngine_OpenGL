#include "Render_Water.h"
#include "Parse_Shader.h"
#include "Var_Render.h"
#include "Light.h"
#include "Framebuffer.h"

using namespace glm;

Render_Water::Render_Water() : moveFactor(0.0f) {
	mesh_water.resize(1);
	vao.resize(1);
	vbo_mesh.resize(1);
	vbo_normal.resize(1);
	vbo_uv.resize(1);
	varRender = new Var_Render;
}

Render_Water::~Render_Water() {
	glDeleteVertexArrays(1, &vao[0]);
	glDeleteBuffers(1, &vbo_mesh[0]);
	glDeleteBuffers(1, &vbo_normal[0]);
	glDeleteBuffers(1, &vbo_uv[0]);
	glDeleteTextures(1, &texture_dudv);
	glDeleteTextures(1, &texture_normal);
	delete varRender;
}

void Render_Water::InitWaterRenderer() {
	GenShaderProgram();
	GetShaderVar();
	GenBuffers();
	GenTextures();
}

void Render_Water::AddWater(const vec2 &xzScale, const vec3 &offset) {
	mesh_water[0].SetMesh(xzScale, offset);
	BindBufferData(0);
}

float Render_Water::GetHeight() {
	return mesh_water[0].mesh[0].y;
}

void Render_Water::GenBuffers() {
	glGenVertexArrays(1, &vao[0]);
	glGenBuffers(1, &vbo_mesh[0]);
	glGenBuffers(1, &vbo_normal[0]);
	glGenBuffers(1, &vbo_uv[0]);
}

void Render_Water::Draw(const mat4 &ViewProjectionMatrix) {
	varRender->ModelViewProjectionMatrix = ViewProjectionMatrix;
	glUseProgram(shaderProgram[0]);
	SetShaderVar();

	
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Render_Water::BindBufferData(const int &index) {
	glBindVertexArray(vao[index]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_water[index].mesh), mesh_water[index].mesh, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_water[index].uv), mesh_water[index].uv, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(2);
}

void Render_Water::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Water.vert" },
		{ GL_FRAGMENT_SHADER, "Shader/Water.frag" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
}

void Render_Water::GetShaderVar() {
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_numLight = glGetUniformLocation(shaderProgram[0], "numLight");
	varRender->loc_light_intensity = glGetUniformLocation(shaderProgram[0], "light_intensity");
	varRender->loc_light_position = glGetUniformLocation(shaderProgram[0], "light_position");
	varRender->loc_light_direction = glGetUniformLocation(shaderProgram[0], "light_direction");
	varRender->loc_color_ambient = glGetUniformLocation(shaderProgram[0], "color_ambient");
	varRender->loc_color_diffuse = glGetUniformLocation(shaderProgram[0], "color_diffuse");
	varRender->loc_color_specular = glGetUniformLocation(shaderProgram[0], "color_specular");
	varRender->loc_eye_position = glGetUniformLocation(shaderProgram[0], "eye_position");
	varRender->loc_tex_reflect = glGetUniformLocation(shaderProgram[0], "tex_reflect");
	varRender->loc_tex_trans = glGetUniformLocation(shaderProgram[0], "tex_trans");
	varRender->loc_tex_depth = glGetUniformLocation(shaderProgram[0], "tex_depth");
	varRender->loc_tex_dudv = glGetUniformLocation(shaderProgram[0], "tex_dudv");
	varRender->loc_tex_normal = glGetUniformLocation(shaderProgram[0], "tex_normal");
	varRender->loc_moveFactor = glGetUniformLocation(shaderProgram[0], "moveFactor");
}

void Render_Water::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
	glUniform1i(varRender->loc_numLight, light->numLight);
	glUniform1fv(varRender->loc_light_intensity, light->numLight, &light->intensity[0]);
	glUniform3fv(varRender->loc_light_position, light->numLight, &light->position[0][0]);
	glUniform3fv(varRender->loc_light_direction, light->numLight, &light->direction[0][0]);
	glUniform3fv(varRender->loc_eye_position, 1, &varRender->eyePosition[0]);
	glUniform1f(varRender->loc_moveFactor, moveFactor);
	moveFactor += WAVESPEED;
}

void Render_Water::SetLight(Light *light) {
	this->light = light;
}

void Render_Water::SetFramebuffer(Framebuffer *framebuffer) {
	this->framebuffer = framebuffer; 
}

void Render_Water::GenTextures() {
	glGenTextures(1, &texture_dudv);
	glActiveTexture(GL_TEXTURE0 + texture_dudv);
	glBindTexture(GL_TEXTURE_2D, texture_dudv);
	LoadTexture("Res/Texture/Water/dudv.png");
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glGenTextures(1, &texture_normal);
	glActiveTexture(GL_TEXTURE0 + texture_normal);
	glBindTexture(GL_TEXTURE_2D, texture_normal);
	LoadTexture("Res/Texture/Water/normal.png");
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glUseProgram(shaderProgram[0]);
	glUniform1i(varRender->loc_tex_reflect, framebuffer->colorTex_water_reflect);
	glUniform1i(varRender->loc_tex_trans, framebuffer->colorTex_water_refract);
	glUniform1i(varRender->loc_tex_depth, framebuffer->depthTex_water_refract);
	glUniform1i(varRender->loc_tex_dudv, texture_dudv);
	glUniform1i(varRender->loc_tex_normal, texture_normal);
	glUseProgram(0);
}

void Render_Water::UpdateEyePos(const glm::vec3 &pos) {
	varRender->eyePosition = pos;
}