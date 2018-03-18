#ifndef __MODEL_MATERIAL_H
#define __MODEL_MATERIAL_H

#include <string>

#include <glm/glm.hpp>
#include <assimp/scene.h>

#define COLOR_AMBIENT 0
#define COLOR_DIFFUSE 1
#define COLOR_SPECULAR 2

class Model_Material {
public:
	Model_Material();
	~Model_Material();

	std::string texture_diffuse;
	std::string texture_normal;
	glm::vec4 color_ambient;
	glm::vec4 color_diffuse;
	glm::vec4 color_specular;

	unsigned int tbo_diffuse;
	unsigned int tbo_normal;
	bool useTexture;
	
	void CopyColor(aiColor4D color, int flag);
	void CopyTexture(const unsigned int &tbo);

private:

};
#endif