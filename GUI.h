#ifndef __GUI_H
#define __GUI_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Button.h"
#include "Font.h"
#include "Text.h"
#include "ProgressBar.h"
#include "ProgressBar_Tex.h"

class Render_Button;
class Render_Text;
class Render_ProgressBar;
class Editor_Terrain;

enum Index_GUI_Greetings { GRT_MAINSCREEN, GRT_ENT_OBJE, GRT_ENT_TERE, 
	GRT_SIZE };
enum Index_GUI_Loading { LOAD_MAINSCREEN, LOAD_SIZE };
enum Index_GUI_ObjectEditor { OBJE_CHATBOX, OBJE_CHATIN, OBJE_BT_SAVE,
	OBJE_FPS, OBJE_ABAR_WRAP, OBJE_SIZE };
enum Index_ABar_ObjectEditor { ABAR_OBJE_SCALE, ABAR_OBJE_R_X, ABAR_OBJE_R_Y,
	ABAR_OBJE_R_Z, ABAR_OBJE_P_Y, ABAR_OBJE_SIZE };
enum Index_GUI_TerrainEditor { TERE_ABAR_WRAP, TERE_BUTTON_SAVE, TERE_SIZE };
enum Index_Abar_TerrainEditor { ABAR_TERE_BRUSHSIZE, ABAR_TERE_OPACITY, ABAR_TERE_SIZE };

enum Index_Font { FONT_GULIM, FONT_SIZE };

class GUI {
public:
	GUI(const glm::vec2 &viewport);
	~GUI();

	void EnterScene(const int &scene);
	void Cleanup();
	void Scene_Greetings();
	void Scene_Loading();
	void Scene_TerrainEditor();
	void Scene_ObjectEditor();
	void Activate();
	void Deactivate();
	void SetRender_Button(Render_Button *render_button);
	void SetRender_Text(Render_Text *render_text);
	void SetRender_ProgressBar(Render_ProgressBar *render_progressBar);
	void SetViewport(const glm::vec2 &viewport);
	void SetEditor_Terrain(Editor_Terrain *editor_terrain);
	void DrawButton();
	void DrawTexts();
	void DrawProgressBar();
	void DrawAdjustBar();
	void GenProgressBarTexture(ProgressBar_Tex &pBar, std::string filePath_foreground, std::string filePath_background, 
		std::string filePath_effect, std::string filePath_cursor);
	void GenButtonTexture(Button &button, std::string filePath_background, std::string filePath_border);
	int ButtonClickTest(const glm::vec2 &mouseCoord);
	void MoveActivatedBar(const int &moveX);
	bool ButtonUnderMouse(const glm::vec2 &mouseCoord);
	bool AdjustBarClickTest(const glm::vec2 &mouseCoord);
	void SendText(const int &index, std::string msg);
	void SendText(const int &index, std::wstring msg);
	void SendChar(const int &index, const int &character);
	void ReleaseText(const int &textBoxIndex, const int &msgIndex);
	void DeleteChar(const int &textBoxIndex);
	std::wstring GetCurrentMessage(const int &textBoxIndex);
	void HighlightBox(const int &boxIndex, const bool &tf);
	void AttachProperty(float *target, const float &minVal, const float &maxVal, const int &barIndex);
	void DetachProperty(const int &barIndex);
	void UpdateAdjustBars();

	ProgressBar *aBar_activated;

private:
	void DrawTexts(std::vector<Text> *textPtr);

	Render_Button *render_button;
	Render_Text *render_text;
	Render_ProgressBar *render_progressBar;
	Editor_Terrain *editor_terrain;
	glm::vec2 viewport;
	glm::vec2 origin; // Left-down point coord
	glm::vec2 size;
	bool isActive;
	
	std::vector<Button> boxes;
	std::list<Button*> reactiveButtons;
	std::vector<Font> fonts;
	std::vector<Text> texts;
	std::vector<ProgressBar> pBars;
	std::vector<ProgressBar> aBars;
	std::vector<Text> aBars_text;
	std::vector<Text> aBars_name;
	std::vector<ProgressBar_Tex> pBar_tex;
	std::vector<ProgressBar_Tex> aBar_tex;
	std::string filePath_progressBar;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 ViewProjection;
};
#endif