#ifndef __OBJECT_FINDER_H
#define __OBJECT_FINDER_H

#include <list>
#include <GL/glew.h>
#include "Object_Info.h"

class Render_Models_Static;
class Render_Models_Animated;

#define ERR_RAYCAST -1

class ObjectFinder {
public:
	ObjectFinder();
	~ObjectFinder();
	
	void SetRenderer(Render_Models_Static *rs, Render_Models_Animated *ra);
	void AddObject(ObjectInfo *object);
	void RemoveObject(ObjectInfo *object);
	std::list<ObjectInfo*>::iterator FindObject(ObjectInfo *object);
	ObjectInfo* FindObjectRaycast(const glm::vec3 &origin, const glm::vec3 &ray, const float &delta, const float &range);
	std::list<ObjectInfo*> objectList;

private:
	void CalcNewCuboid(ObjectInfo*);

	std::list<ObjectInfo*>::iterator objIter;
	SelectionBox selectionBox;
	ObjectInfo* selectedObject;
	Render_Models_Static *rms;
	Render_Models_Animated *rma;
};
#endif