#ifndef __SELECTIONBOX_H
#define __SELECTIONBOX_H

#include <vector>
#include <GL/glew.h>

struct SelectionBox {
	float x_max;
	float x_min;
	float y_max;
	float y_min;
	float z_max;
	float z_min;
	glm::vec3 vertices[8];
};

#endif