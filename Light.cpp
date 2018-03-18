#include "Light.h"

Light::Light() : numLight(0) {

}

Light::~Light() {

}

void Light::AddLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity) {
	this->position.push_back(position);
	this->direction.push_back(direction);
	this->color.push_back(color);
	this->intensity.push_back(intensity);
	numLight = this->position.size();
}