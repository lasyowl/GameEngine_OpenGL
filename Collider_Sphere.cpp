#include "Collider_Sphere.h"

#include <stdio.h>
#include <gl/glew.h>

using namespace glm;
using namespace std;

Collider_Sphere::Collider_Sphere() {

}

Collider_Sphere::~Collider_Sphere() {

}

int Collider_Sphere::CollisionTest(Collider_Sphere *collider) {

	float dist = distance(this->center, collider->center);
	// Case 0 : No collision
	if (dist > this->radius + collider->radius) {
		switch (collider->trigStatus) {
		case ENTER_COLLISION:
		case WHILE_COLLISION:
			collider->trigStatus = EXIT_COLLISION;
			break;
		case EXIT_COLLISION:
			collider->trigStatus = NO_COLLISION;
			break;
		default:
			break;
		}
	}
	// Case 1 : On collision
	else {
		switch (collider->trigStatus) {
		case ENTER_COLLISION:
		case WHILE_COLLISION:
			collider->trigStatus = WHILE_COLLISION;
			break;
		case EXIT_COLLISION:
			collider->trigStatus = ENTER_COLLISION;
			break;
		case NO_COLLISION:
			collider->trigStatus = ENTER_COLLISION;
			break;
		default:
			break;
		}
	}

	return collider->trigStatus;
}

void Collider_Sphere::UpdateVariables(const vec3 &center, const float &radius) {
	this->center = center;
	this->radius = radius;
}