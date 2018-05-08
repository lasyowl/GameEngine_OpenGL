#ifndef __GUI_HUD_H
#define __GUI_HUD_H

class Character;

class GUI_HUD {
public:
	GUI_HUD();
	~GUI_HUD();
	void SetReference(Character *character);

private:
	Character *character;
};
#endif