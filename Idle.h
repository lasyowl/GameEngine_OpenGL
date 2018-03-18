#ifndef __IDLE_H
#define __IDLE_H

class GameObject;
class Flags_IO;

class Idle {
public:
	Idle();
	~Idle();

	void Initiate(GameObject *gameObject, Flags_IO *flags_IO);

	static void Update();

private:
	void Update_Greetings();
	void Update_ObjectEditor();
	void Update_TerrainEditor();
	
	void PlayerMove();
	void CameraMove();

	static Idle *me;
	GameObject *gameObject;
	Flags_IO *flags_io;

};
#endif