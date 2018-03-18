#include "Render_Particles.h"
#include "Parse_Shader.h"
#include "Var_Compute.h"
#include "Var_Render.h"

using namespace glm;

Render_Particles::Render_Particles() {
	InitParticleRenderer();
}

Render_Particles::~Render_Particles() {

}

void Render_Particles::InitParticleRenderer() {
	GenShaderProgram();
	GetShaderVar();
	GenTextureObjects();
}

void Render_Particles::GenShaderProgram() {
	ShaderInfo shaderInfo_default[MAX_SHADER_NUMBER] = {
		{ GL_VERTEX_SHADER, "Shader/Particle.vert" },
		{ GL_GEOMETRY_SHADER, "Shader/Particle.geom" },
		{ GL_FRAGMENT_SHADER, "Shader/Particle.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shaderInfo_compute[MAX_SHADER_NUMBER] = {
		{ GL_COMPUTE_SHADER, "Shader/Particle.comp" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};

	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_default));
	shaderProgram.push_back(parseShader->LoadShader(shaderInfo_compute));
}

void Render_Particles::GetShaderVar() {
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
	varRender->loc_texture_array = glGetUniformLocation(shaderProgram[0], "texture_array");
	glUniform1i(varRender->loc_texture_array, 0);

	glUseProgram(shaderProgram[1]);
	varCompute = new Var_Compute;
	varCompute->loc_texture_random = glGetUniformLocation(shaderProgram[1], "texture_random");
	varCompute->loc_particle_size = glGetUniformLocation(shaderProgram[1], "particle_size");
	glUniform1i(varCompute->loc_texture_random, 0);
	glUniform1i(varCompute->loc_particle_size, MAX_NUM_PARTICLES);
}

void Render_Particles::GenTextureObjects() {
	glGenTextures(1, &randTexture);
	glBindTexture(GL_TEXTURE_1D, randTexture);
	GenRandomTexture(128);
	glGenTextures(1, &texture_particle);
	glActiveTexture(GL_TEXTURE_2D + texture_particle);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_particle);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 256, 256, 10);
	LoadTextureArray("Res/Texture/Particle/exp2_0.png", 0);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void Render_Particles::SetShaderVar() {
	glUniformMatrix4fv(varRender->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &varRender->ModelViewProjectionMatrix[0][0]);
}

void Render_Particles::GenObjects(Particles *particle) {
	glGenVertexArrays(1, &particle->vao);
	glGenBuffers(1, &particle->vbo_vertex);
}

void Render_Particles::BindObjectData(Particles *particle) {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle->vbo_vertex);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particle->info.size() * sizeof(Info_Particle), &particle->info[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(particle->vao);

	glBindBuffer(GL_ARRAY_BUFFER, particle->vbo_vertex);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Info_Particle), (GLvoid*)sizeof(vec4));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, particle->vbo_vertex);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Info_Particle), (GLvoid*)(sizeof(vec4) * 3));
	glEnableVertexAttribArray(1);
}

void Render_Particles::DispatchCS() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, randTexture);

	glUseProgram(shaderProgram[1]);
	std::list<Particles>::iterator p = particles.begin();
	int index = 0;
	
	while(index < particles.size()){
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, (*p).vbo_vertex);
		glDispatchCompute((*p).info.size(), 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		float type;
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float), &type);
		if (type == TYPE_DESTROY) {
			glDeleteBuffers(1, &(*p).vbo_vertex);
			glDeleteVertexArrays(1, &(*p).vao);
			particles.erase(p);
			break;
		}
		p++;
		index++;
	}
}

void Render_Particles::AddParticle(int timeRemain, vec3 position, vec3 velocity, int particleNum) {
	Particles particle_temp(timeRemain, position, velocity, particleNum);
	GenObjects(&particle_temp);
	particles.push_back(particle_temp);
	std::list<Particles>::iterator p = --particles.end();
	BindObjectData(&*p);
}

void Render_Particles::Draw(mat4 ViewProjectionMatrix) {
	// Early return
	if (particles.size() == 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	DispatchCS();
	glUseProgram(shaderProgram[0]);
	varRender->ModelViewProjectionMatrix = ViewProjectionMatrix;
	SetShaderVar();
	std::list<Particles>::iterator p = particles.begin();
	
	for (; p != particles.end(); p++) {
		glBindVertexArray((*p).vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture_particle);
		glDrawArrays(GL_POINTS, 0, (*p).info.size());
	}
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}