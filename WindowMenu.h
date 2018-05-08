#ifndef __WINDOW_MENU_H
#define __WINDOW_MENU_H

class ObjectFinder;
class ModelTransform_Loader;
class Controller_Character;
class Controller_Static;
class Flags_IO;

class WindowMenu {
public:
	WindowMenu();
	~WindowMenu();

	void Initiate(Flags_IO *flags_io);
	void CreateEditorMenu();
	void SetObjFinder(ObjectFinder *objFinder);
	void SetDatabase(ModelTransform_Loader *database);
	void SetControllers(Controller_Character *controller_character, Controller_Static *controller_static);

private:
	static WindowMenu *me;

	int GenObjectLoaderMenu();
	static void RunObjectLoaderMenu(int menuHandle);
	static void RunAnimatedObjectMenu_Player(int menuHandle);
	static void RunAnimatedObjectMenu_Enemy(int menuHandle);
	static void RunStaticObjectMenu(int menuHandle);

	ObjectFinder *objFinder;
	ModelTransform_Loader *database;
	Controller_Character *controller_character;
	Controller_Static *controller_static;
	Flags_IO *flags_io;
};
#endif