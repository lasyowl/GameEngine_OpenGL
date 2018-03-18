#ifndef __RENDER_WATER_H
#define __RENDER_WATER_H

#include "Render.h"
#include "Mesh_Water.h"

#include <vector>

class Light;
class Framebuffer;

#define WAVESPEED 0.0005f;

class Render_Water : public Render {
public:
	Render_Water();
	~Render_Water();

	void AddWater(const glm::vec2 &xzScale, const glm::vec3 &offset);
	void Draw(const glm::mat4 &ViewProjectionMatrix);
	void InitWaterRenderer();
	void SetLight(Light *light);
	void SetFramebuffer(Framebuffer *framebuffer);
	float GetHeight();
	void UpdateEyePos(const glm::vec3 &pos);

private:
	void GenBuffers();
	void BindBufferData(const int &index);
	void GenShaderProgram();
	void GetShaderVar();
	void SetShaderVar();
	void GenTextures();

	Light *light;
	Framebuffer *framebuffer;

	std::vector<Mesh_Water> mesh_water;
	std::vector<GLuint> vao;
	std::vector<GLuint> vbo_mesh;
	std::vector<GLuint> vbo_normal;
	std::vector<GLuint> vbo_uv;
	
	GLuint texture_dudv;
	GLuint texture_normal;

	float moveFactor;
};
#endif