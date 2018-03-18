#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

enum Enum_Framebuffer { FB_WATER_REFLECT, FB_WATER_REFRACT, FB_SHADOWMAP, FB_POSTPROCESS0, FB_POSTPROCESS1, FB_POSTPROCESS2, FB_SIZE };

enum Enum_TexNumber { TEX_POSTPROCESS0, TEX_POSTPROCESS1, TEX_POSTPROCESS2, TEX_POSTPROCESS_SIZE };

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();

	void InitFramebuffer();
	void BindFramebuffer(const int &index);
	void SetViewport(glm::vec2 *viewport);

	GLuint colorTex_water_reflect;
	GLuint colorTex_water_refract;
	GLuint depthTex_shadowmap;
	GLuint depthTex_water_reflect;
	GLuint depthTex_water_refract;
	GLuint colorTex_postProcess[TEX_POSTPROCESS_SIZE];
	GLuint depthTex_postProcess[TEX_POSTPROCESS_SIZE];
	glm::vec2 *viewport;

private:
	GLuint fbo[FB_SIZE];

};
#endif