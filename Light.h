#ifndef __LIGHT_H
#define __LIGHT_H

#include <vector>
#include <glm/glm.hpp>

class Light {
public:
	Light();
	~Light();

	void AddLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity);
	void SetDirection(const int &index, const glm::vec3 &direction);
	void SetIntensity(const int &index, const float &intensity);

	std::vector<float> intensity;
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> direction;
	std::vector<glm::vec3> color;
	int numLight;

private:

};

#endif