#include "Render_Particles_V2.h"

#include "Parse_Shader.h"
#include "Var_Render.h"

using namespace glm;

Render_Particles_V2::Render_Particles_V2() {

}

Render_Particles_V2::~Render_Particles_V2() {

}

void Render_Particles_V2::Initiate() {
	GenShaderProgram();
	GetShaderVar();
}

void Render_Particles_V2::Draw(const mat4 &MVP, const GLuint &vao, const int &drawCount, const GLuint &tex) {
	varRender->ModelViewProjectionMatrix = MVP;
	this->tex = tex;

	//glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glUseProgram(shaderProgram[0]);
	glBindVertexArray(vao);

	SetShaderVar();

	glDrawArrays(GL_POINTS, 0, drawCount);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render_Particles_V2::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Particle_V2.vert" },
		{ GL_GEOMETRY_SHADER, "Shader/Particle_V2.geom" },
		{ GL_FRAGMENT_SHADER, "Shader/Particle_V2.frag" },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
}

void Render_Particles_V2::GetShaderVar() {
	varRender = new Var_Render;
	varRender->loc_ModelViewProjectionMatrix = glGetUniformLocation(shaderProgram[0], "ModelViewProjectionMatrix");
	varRender->loc_texture_diffuse = glGetUniformLocation(shaderProgram[0], "tex_atlas");
	varRender->loc_eye_position = glGetUniformLocation(shaderProgram[0], "eyePosition");
	varRender->loc_width = glGetUniformLocation(shaderProgram[0], "width");
	varRender->loc_height = glGetUniformLocation(shaderProgram[0], "height");
	varRender->loc_spriteNum = glGetUniformLocation(shaderProgram[0], "spriteNum");
	varRender->loc_cycleTime = glGetUniformLocation(shaderProgram[0], "cycleTime");
	varRender->loc_loop = glGetUniformLocation(shaderProgram[0], "loop");
	varRender->loc_intensity = glGetUniformLocation(shaderProgram[0], "intensity");
}

void Render_Particles_V2::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
	glUniform1i(varRender->loc_texture_diffuse, tex);
	glUniform3fv(varRender->loc_eye_position, 1, &eyePosition[0]);
	glUniform1i(varRender->loc_width, width);
	glUniform1i(varRender->loc_height, height);
	glUniform1f(varRender->loc_spriteNum, spriteNum);
	glUniform1f(varRender->loc_cycleTime, cycleTime);
	glUniform1i(varRender->loc_loop, loop);
	glUniform1f(varRender->loc_intensity, intensity);
}