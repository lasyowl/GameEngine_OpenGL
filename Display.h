#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <glm/glm.hpp>

#include "GlobalFunctions.h"

class GameObject;
class Flags_IO;
class Terrain;

class Display {
	enum ShadowBox { NLD, NLU, NRD, NRU, FLD, FLU, FRD, FRU };
public:
	Display();
	~Display();

	void Initiate(GameObject *gameObject, Flags_IO *flags_io);

	static void Draw();
	static void Resize(int x, int y);

private:
	void Draw_Greetings();
	void Draw_Loading_ObjectEditor();
	void Draw_ObjectEditor();
	void Draw_TerrainEditor();
	void Draw_Gameplay();
	void DrawOffscreen_Water();
	void DrawOffscreen_Shadowmap();
	void DrawTerrain(Terrain *terrain, const glm::mat4 &VP);
	void DrawTerrain_R(Terrain *terrain);
	void DrawSun();
	void DrawOnscreen();
	void DrawGUI();
	void DrawPostprocess();
	void CalcShadowbox();
	void FPSCounter_ObjectEditor();
	void FPSCounter_Gameplay();

	static Display *me;

	GameObject *gameObject;
	Flags_IO *flags_io;

	GlobalFunctions gf;
	float dod = 0.0f;
	float mom = 0.0f;

	glm::mat4 VP;
	glm::mat4 VP_sky;
	glm::mat4 VP_shadow;
	glm::mat4 ModelMatrix;
	glm::vec4 shadowBox[8];
};
#endif