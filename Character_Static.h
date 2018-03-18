#ifndef __CHARACTER_STATIC_H
#define __CHARACTER_STATIC_H

#include "Character.h"

class Character_Static :public Character {
public:
	Character_Static();
	~Character_Static();

	bool operator == (const Character_Static &rightHand);

	void Initiate(const ObjectInfo &objectInfo, Render_Models_Static *models_static, Mesh_Terrain *terrain);
	bool Activate();

private:
	void SetRenderer(Render_Models_Static *renderer);
	Render_Models_Static *render_static;
};
#endif