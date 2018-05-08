#ifndef __COLLIDER_SPHERE_H
#define __COLLIDER_SPHERE_H

#include "Collider.h"

class Collider_Sphere : public Collider {
public:
	Collider_Sphere();
	virtual ~Collider_Sphere();

	int CollisionTest(Collider_Sphere *collider);
	void UpdateVariables(const glm::vec3 &center, const float &radius);

	glm::vec3 center;
	float radius;

private:

};
#endif