#include "Controller_Static.h"
#include "ObjectFinder.h"

using namespace std;

Controller_Static::Controller_Static() {

}

Controller_Static::~Controller_Static() {

}

void Controller_Static::InitController(Render_Models_Static *models_static, Mesh_Terrain *terrain) {
	this->render_models_static = models_static;
	this->mesh_terrain = terrain;
}

Character_Static* Controller_Static::AddCharacter(const ObjectInfo &objectInfo, ObjectFinder *objectFinder) {
	Character_Static temp;
	temp.Initiate(objectInfo, render_models_static, mesh_terrain);
	temp.objectFinder = objectFinder;
	list<Character_Static>::iterator iter = characters.end();
	characters.insert(iter, temp);
	--iter;
	if (iter->Activate() == CHAR_ACTIV_FAIL)
		characters.erase(iter);
	iter->iter->character = &(*iter);
	iter->objectFinder->AddObject(&(*iter->iter));
	printf("%d static characters exists\n", characters.size());
	return &*iter;
}

void Controller_Static::DeleteCharacter(Character_Static *target) {
	if (characters.empty()) {
		printf("No objects\n");
		return;
	}
	target->objectFinder->RemoveObject(&(*target->iter));
	list<Character_Static>::iterator targetIter = find(characters.begin(), characters.end(), *target);

	characters.erase(targetIter);
	printf("%d characters\n", characters.size());
}

void Controller_Static::UpdateCharacters() {
	list<Character_Static>::iterator iter = characters.begin();
	while (iter != characters.end()) {
		iter->Update();
		++iter;
	}
}