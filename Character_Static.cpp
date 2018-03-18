#include "Character_Static.h"

Character_Static::Character_Static() {

}

Character_Static::~Character_Static() {
	isInherited = true;
	if (isInitiated == true)
		render_static->DeleteObject(iter);
}

bool Character_Static::operator == (const Character_Static &rightHand) {
	if (&rightHand == this)
		return true;
	else return false;
}

void Character_Static::Initiate(const ObjectInfo &objectInfo, Render_Models_Static *models_static, Mesh_Terrain *terrain) {
	SetRenderer(models_static);
	SetTerrain(terrain);
	SetObjectInfo(objectInfo);
	SetPos(objectInfo.pos);
	SetDir(objectInfo.dir);
	SetScale(objectInfo.scale);
	SetRMatrix(objectInfo.RMatrix);
	SetPreRot(objectInfo.preRot);
	SetTag(objectInfo.tag);
	SetModelId(objectInfo.modelId);
	GenPreRotMatrix();
}

bool Character_Static::Activate() {
	iter = render_static->AddObject(objectInfo);
	if (iter == render_static->IsFull()) {
		printf("Activation fail\n");
		return CHAR_ACTIV_FAIL;
	}

	isInitiated = true;
	direction = dir_origin;
	return CHAR_ACTIV_SUCCESS;
}

void Character_Static::SetRenderer(Render_Models_Static *renderer) {
	render_static = renderer;
}