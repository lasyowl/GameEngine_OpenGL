#include "Model_Material.h"

#include <GL/freeglut.h>

Model_Material::Model_Material() :tbo_diffuse(-1), useTexture(false) {

}

Model_Material::~Model_Material() {
	glDeleteTextures(1, &tbo_diffuse);
}

void Model_Material::CopyColor(aiColor4D color, int flag) {
	switch (flag) {
		case COLOR_AMBIENT:
			color_ambient.r = color.r;
			color_ambient.g = color.g;
			color_ambient.b = color.b;
			color_ambient.a = color.a;
			break;
		case COLOR_DIFFUSE:
			color_diffuse.r = color.r;
			color_diffuse.g = color.g;
			color_diffuse.b = color.b;
			color_diffuse.a = color.a;
			break;
		case COLOR_SPECULAR:
			color_specular.r = color.r;
			color_specular.g = color.g;
			color_specular.b = color.b;
			color_specular.a = color.a;
			break;
		default:
			break;
	}
}

void Model_Material::CopyTexture(const unsigned int &tbo) {
	tbo_diffuse = tbo;
}