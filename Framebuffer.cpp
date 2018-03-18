#include "Framebuffer.h"

#include <stdio.h>

Framebuffer::Framebuffer() {

}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(FB_SIZE, fbo);
	glDeleteTextures(1, &colorTex_water_reflect);
	glDeleteTextures(1, &colorTex_water_refract);
	glDeleteTextures(1, &depthTex_water_reflect);
	glDeleteTextures(1, &depthTex_water_refract);
	glDeleteTextures(1, &depthTex_shadowmap);
}

void Framebuffer::InitFramebuffer() {
	GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };

	glGenFramebuffers(FB_SIZE, fbo);
	glGenTextures(1, &colorTex_water_reflect);
	glGenTextures(1, &colorTex_water_refract);
	glGenTextures(1, &depthTex_water_reflect);
	glGenTextures(1, &depthTex_water_refract);
	glGenTextures(1, &depthTex_shadowmap);
	glGenTextures(TEX_POSTPROCESS_SIZE, colorTex_postProcess);
	glGenTextures(TEX_POSTPROCESS_SIZE, depthTex_postProcess);

	// Gen FBO for reflect texture
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[FB_WATER_REFLECT]);

	glBindTexture(GL_TEXTURE_2D, colorTex_water_reflect);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, viewport->x, viewport->y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTex_water_reflect, 0);

	glBindTexture(GL_TEXTURE_2D, depthTex_water_reflect);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, viewport->x, viewport->y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex_water_reflect, 0);

	glDrawBuffers(1, draw_buffers);

	// Gen FBO for transparent texture
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[FB_WATER_REFRACT]);

	glBindTexture(GL_TEXTURE_2D, colorTex_water_refract);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, viewport->x, viewport->y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTex_water_refract, 0);

	glBindTexture(GL_TEXTURE_2D, depthTex_water_refract);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, viewport->x, viewport->y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex_water_refract, 0);

	glDrawBuffers(1, draw_buffers);

	// Gen FBO for shadowmap
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[FB_SHADOWMAP]);

	glBindTexture(GL_TEXTURE_2D, depthTex_shadowmap);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, viewport->x * 8, viewport->y * 8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex_shadowmap, 0);

	glDrawBuffer(GL_NONE);

	// Gen FBO for postprocess
	for (int i = 0; i < TEX_POSTPROCESS_SIZE; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo[FB_POSTPROCESS0 + i]);

		glBindTexture(GL_TEXTURE_2D, colorTex_postProcess[TEX_POSTPROCESS0 + i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, viewport->x, viewport->y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTex_postProcess[TEX_POSTPROCESS0 + i], 0);

		if (i == TEX_POSTPROCESS0) {
			glBindTexture(GL_TEXTURE_2D, depthTex_postProcess[TEX_POSTPROCESS0 + i]);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, viewport->x, viewport->y);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex_postProcess[TEX_POSTPROCESS0 + i], 0);
		}
		glDrawBuffers(1, draw_buffers);

		glActiveTexture(GL_TEXTURE0 + colorTex_postProcess[TEX_POSTPROCESS0 + i]);
		glBindTexture(GL_TEXTURE_2D, colorTex_postProcess[TEX_POSTPROCESS0 + i]);
	}
	glActiveTexture(GL_TEXTURE0 + colorTex_water_reflect);
	glBindTexture(GL_TEXTURE_2D, colorTex_water_reflect);

	glActiveTexture(GL_TEXTURE0 + colorTex_water_refract);
	glBindTexture(GL_TEXTURE_2D, colorTex_water_refract);

	glActiveTexture(GL_TEXTURE0 + depthTex_water_refract);
	glBindTexture(GL_TEXTURE_2D, depthTex_water_refract);

	glActiveTexture(GL_TEXTURE0 + depthTex_shadowmap);
	glBindTexture(GL_TEXTURE_2D, depthTex_shadowmap);

	for (int i = 0; i < TEX_POSTPROCESS_SIZE; i++) {
		glActiveTexture(GL_TEXTURE0 + colorTex_postProcess[TEX_POSTPROCESS0 + i]);
		glBindTexture(GL_TEXTURE_2D, colorTex_postProcess[TEX_POSTPROCESS0 + i]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::BindFramebuffer(const int &index) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo[index]);
}

void Framebuffer::SetViewport(glm::vec2 *viewport) {
	this->viewport = viewport;
}