#ifndef __CONTROLLER_STATIC_H
#define __CONTROLLER_STATIC_H

#include "Character_Static.h"
#include "Controller_Object.h"


class Controller_Static : public Controller_Object {
public:
	Controller_Static();
	~Controller_Static();

	void InitController(Render_Models_Static *renderer, Mesh_Terrain *render_terrain);
	Character_Static* AddCharacter(const ObjectInfo &objectInfo, ObjectFinder *objectFinder);
	void DeleteCharacter(Character_Static *target);
	void UpdateCharacters();

private:
	Render_Models_Static *render_models_static;

	std::list<Character_Static> characters;
};
#endif