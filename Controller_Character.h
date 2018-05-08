#ifndef __CONTROLLER_CHARACTER_H
#define __CONTROLLER_CHARACTER_H

#include "Controller_Object.h"

class Controller_Character : public Controller_Object {
public:
	Controller_Character();
	~Controller_Character();

	void InitController(Render_Models_Animated *renderer, Mesh_Terrain *render_terrain);
	Character* AddCharacter(const ObjectInfo &objectInfo, ObjectFinder *objectFinder);
	void DeleteCharacter(Character *target);
	void UpdateCharacters();

	std::list<Character> characters;

private:
	Render_Models_Animated *render_models_animated;

};
#endif