#include "ObjectFinder.h"
#include "Render_Models_Animated.h"
#include "Render_Models_Static.h"

#include <algorithm>

using namespace std;
using namespace glm;

ObjectFinder::ObjectFinder() : selectedObject(nullptr) {

}

ObjectFinder::~ObjectFinder() {

}

void ObjectFinder::SetRenderer(Render_Models_Static *rs, Render_Models_Animated *ra) {
	rms = rs;
	rma = ra;
}

void ObjectFinder::AddObject(ObjectInfo *object) {
	objectList.push_back(object);
}

void ObjectFinder::RemoveObject(ObjectInfo *object) {
	objIter = FindObject(object);
	if (objIter != objectList.end())
		objectList.erase(objIter);
}

list<ObjectInfo*>::iterator ObjectFinder::FindObject(ObjectInfo *object) {
	return find(objectList.begin(), objectList.end(), object);
}

ObjectInfo* ObjectFinder::FindObjectRaycast(const vec3 &origin, const vec3 &ray, const float &delta, const float &range) {
	bool detected = false;
	vec3 newOrigin = origin;
	vec3 deltaRay = delta * ray;
	int iteration = range / delta;
	if (selectedObject != nullptr)
		selectedObject->isSelected = false;
	for (int i = 0; i < iteration; i++) {
		for (objIter = objectList.begin(); objIter != objectList.end(); objIter++) {
			CalcNewCuboid(*objIter);
			if (newOrigin.x > selectionBox.x_min && newOrigin.x < selectionBox.x_max &&
				newOrigin.y > selectionBox.y_min && newOrigin.y < selectionBox.y_max &&
				newOrigin.z > selectionBox.z_min && newOrigin.z < selectionBox.z_max) {
				(*objIter)->isSelected = true;
				selectedObject = *objIter;
				return *objIter;
			}
		}
		newOrigin += deltaRay;
	}
	return nullptr;
}

void ObjectFinder::CalcNewCuboid(ObjectInfo *objInfo) {
	for (int i = 0; i < 8; i++) {
		mat4 correction;
		if (objInfo->tag == 0) {
			correction = rma->scene[objInfo->modelId].rootNode.transformation;
		}
		else if (objInfo->tag == 2) {
			correction = rms->scene[objInfo->modelId].rootNode.transformation;
		}
		selectionBox.vertices[i] = vec3(objInfo->SelectionBoxTransformMatrix * correction * vec4(objInfo->selectionBox->vertices[i], 1.0f));
	}
	selectionBox.x_min = selectionBox.x_max = selectionBox.vertices[0].x;
	selectionBox.y_min = selectionBox.y_max = selectionBox.vertices[0].y;
	selectionBox.z_min = selectionBox.z_max = selectionBox.vertices[0].z;
	for (int i = 0; i < 8; i++) {
		selectionBox.x_min = min(selectionBox.x_min, selectionBox.vertices[i].x);
		selectionBox.x_max = max(selectionBox.x_max, selectionBox.vertices[i].x);
		selectionBox.y_min = min(selectionBox.y_min, selectionBox.vertices[i].y);
		selectionBox.y_max = max(selectionBox.y_max, selectionBox.vertices[i].y);
		selectionBox.z_min = min(selectionBox.z_min, selectionBox.vertices[i].z);
		selectionBox.z_max = max(selectionBox.z_max, selectionBox.vertices[i].z);
	}
}