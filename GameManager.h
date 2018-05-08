#ifndef __GAMEMANAGER_H
#define __GAMEMANAGER_H

#include "Keyboard.h"
#include "Mouse.h"
#include "Display.h"
#include "Idle.h"
#include "WindowMenu.h"
#include "Flags_IO.h"
#include "GlobalFunctions.h"
#include "Character.h"

class GameManager {
public:
	GameManager();
	~GameManager();

	void Initiate(const float &viewportX, const float &viewportY);
	static void IdleFunc();
	static void MenuStatusFunc(int status, int x, int y);
	void Scene_Greetings();
	void Scene_LoadTerrainEditor();
	void Scene_LoadObjectEditor();
	void Scene_LoadGameplay();
	void CleanUp();

	Keyboard keyboard;
	Mouse mouse;
	Display display;
	Idle idle;
	WindowMenu menu;

private:
	static GameManager *me;

	void EnterScene(const int &scene);

	GameObject *gameObject;
	Flags_IO *flags_io;

	int sequence;
};
#endif