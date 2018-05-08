#ifndef __COLLIDER_H
#define __COLLIDER_H

#include <glm/glm.hpp>

enum CollisionStatus { NO_COLLISION, ENTER_COLLISION, WHILE_COLLISION, EXIT_COLLISION };
enum ColliderTag { COLLIDER_NULL, COLLIDER_BLOCK, COLLIDER_RECOGNITION };

class Collider {
public:
	Collider();
	virtual ~Collider();

	int tag;
	int trigStatus;
	bool taskDone;

private:

};
#endif