#ifndef __MOUSE_H
#define __MOUSE_H

class GameObject;
class Flags_IO;
class GUI;
class Camera;

class Mouse {
public:
	Mouse();
	~Mouse();

	void Initiate(GameObject *gameObject, Flags_IO *flags_io);

	static void MousePress(int button, int state, int x, int y);
	static void MouseMoveActive(int x, int y);
	static void MouseMovePassive(int x, int y);
	static void MouseWheel(int wheel, int direction, int x, int y);

private:
	void MousePress_Greetings(const int &button, const int &state, const int &x, const int &y);
	void MousePress_ObjectEditor(const int &button, const int &state, const int &x, const int &y);
	void MousePress_TerrainEditor(const int &button, const int &state, const int &x, const int &y);
	void MouseMoveActive_Greetings(const int &x, const int &y);
	void MouseMoveActive_ObjectEditor(const int &x, const int &y);
	void MouseMoveActive_TerrainEditor(const int &x, const int &y);
	void MouseMovePassive_Greetings(const int &x, const int &y);
	void MouseMovePassive_ObjectEditor(const int &x, const int &y);
	void MouseMovePassive_TerrainEditor(const int &x, const int &y);

	static Mouse *me;
	GameObject *gameObject;
	Flags_IO *flags_io;
	
	GUI *gui;
	Camera *camera;
};
#endif