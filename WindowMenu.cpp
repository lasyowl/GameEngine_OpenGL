#include "WindowMenu.h"

#include "ModelTransform_Loader.h"
#include "Controller_Character.h"
#include "Controller_Static.h"
#include "Flags_IO.h"
#include <stdio.h>
#include <gl/freeglut.h>

WindowMenu* WindowMenu::me;

WindowMenu::WindowMenu() {
	me = this;
}

WindowMenu::~WindowMenu() {

}

void WindowMenu::Initiate(Flags_IO *flags_io) {
	this->flags_io = flags_io;
}

void WindowMenu::SetObjFinder(ObjectFinder *objFinder) {
	this->objFinder = objFinder;
}

void WindowMenu::SetDatabase(ModelTransform_Loader *database) {
	this->database = database;
}

void WindowMenu::SetControllers(Controller_Character *controller_character, Controller_Static *controller_static) {
	this->controller_character = controller_character;
	this->controller_static = controller_static;
}

void WindowMenu::CreateEditorMenu() {
	
	int mainMenu;
	mainMenu = glutCreateMenu(NULL);

	int subMenu_objLoader = GenObjectLoaderMenu();

	glutSetMenu(mainMenu);
	glutAddSubMenu("Add Object", subMenu_objLoader);
}

int WindowMenu::GenObjectLoaderMenu() {
	int menu = glutCreateMenu(RunObjectLoaderMenu);

	int subMenu_animated = glutCreateMenu(RunAnimatedObjectMenu);
	for (int i = 0; i < database->numAnimated; i++) {
		glutAddMenuEntry(database->animatedIndex[i].name.data(), database->animatedIndex[i].index);
	}

	int subMenu_static = glutCreateMenu(RunStaticObjectMenu);
	for (int i = 0; i < database->numStatic; i++) {
		glutAddMenuEntry(database->staticIndex[i].name.data(), database->staticIndex[i].index);
	}

	glutSetMenu(menu);
	glutAddSubMenu("Animated objects", subMenu_animated);
	glutAddSubMenu("Static objects", subMenu_static);
	
	return menu;
}

void WindowMenu::RunObjectLoaderMenu(int menuHandle) {

}

void WindowMenu::RunAnimatedObjectMenu(int menuHandle) {
	ObjectInfo tempObjectInfo;
	tempObjectInfo.modelId = menuHandle;
	tempObjectInfo.pos = me->flags_io->raycastCoord;
	tempObjectInfo.scale = 0.1f;
	tempObjectInfo.dir = glm::vec3(0, 0, -1);
	tempObjectInfo.RMatrix = glm::mat4(1.0f); // Currently not used
	tempObjectInfo.preRot = glm::vec3(0, 0, 0);
	tempObjectInfo.tag = 0;
	tempObjectInfo.yOffset = 0.28f;
	me->controller_character->AddCharacter(tempObjectInfo, me->objFinder);
}

void WindowMenu::RunStaticObjectMenu(int menuHandle) {
	ObjectInfo tempObjectInfo;
	tempObjectInfo.modelId = menuHandle;
	tempObjectInfo.pos = me->flags_io->raycastCoord;
	tempObjectInfo.scale = 0.1f;
	tempObjectInfo.dir = glm::vec3(0, 0, -1);
	tempObjectInfo.RMatrix = glm::mat4(1.0f); // Currently not used
	tempObjectInfo.preRot = glm::vec3(0, 0, 0);
	tempObjectInfo.tag = 2;
	tempObjectInfo.yOffset = 0.28f;
	me->controller_static->AddCharacter(tempObjectInfo, me->objFinder);
}