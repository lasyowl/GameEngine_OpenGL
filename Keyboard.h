#ifndef __KEYBOARD_H
#define __KEYBOARD_H

class GameObject;
class Flags_IO;

class Keyboard {
public:
	Keyboard();
	~Keyboard();

	void Initiate(GameObject *gameObject, Flags_IO *flags_io);
	static void KeyboardUp(unsigned char key, int x, int y);
	static void KeyboardPress(unsigned char key, int x, int y);
	static void SpecialKeyUp(int key, int x, int y);
	static void SpecialKeyPress(int key, int x, int y);

private:
	void KeyboardUp_Greetings(const unsigned char &key, const int &x, const int &y);
	void KeyboardUp_ObjectEditor(const unsigned char &key, const int &x, const int &y);
	void KeyboardUp_TerrainEditor(const unsigned char &key, const int &x, const int &y);
	void KeyboardUp_Gameplay(const unsigned char &key, const int &x, const int &y);
	void KeyboardPress_Greetings(const unsigned char &key, const int &x, const int &y);
	void KeyboardPress_ObjectEditor(const unsigned char &key, const int &x, const int &y);
	void KeyboardPress_TerrainEditor(const unsigned char &key, const int &x, const int &y);
	void KeyboardPress_Gameplay(const unsigned char &key, const int &x, const int &y);
	void SpecialKeyUp_Greetings(const int &key, const int &x, const int &y);
	void SpecialKeyUp_ObjectEditor(const int &key, const int &x, const int &y);
	void SpecialKeyUp_TerrainEditor(const int &key, const int &x, const int &y);
	void SpecialKeyUp_Gameplay(const int &key, const int &x, const int &y);
	void SpecialKeyPress_Greetings(const int &key, const int &x, const int &y);
	void SpecialKeyPress_ObjectEditor(const int &key, const int &x, const int &y);
	void SpecialKeyPress_TerrainEditor(const int &key, const int &x, const int &y);
	void SpecialKeyPress_Gameplay(const int &key, const int &x, const int &y);
	bool MoveKeyReleased();
	void PressTyping_ObjectEditor(const unsigned int &key);
	void PressTyping_Gameplay(const unsigned int &key);
	void PressNotTyping_ObjectEditor(const unsigned char &key);
	void PressNotTyping_Gameplay(const unsigned char &key);

	static Keyboard *me;
	GameObject *gameObject;
	Flags_IO *flags_io;

};
#endif