#include "GUI.h"
#include "Render_Button.h"
#include "Render_Text.h"
#include "Render_ProgressBar.h"
#include "Editor_Terrain.h"
#include "Flags_IO.h"
#include <atlstr.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

GUI::GUI(const vec2 &viewport) : isActive(false), aBar_activated(nullptr) {
	this->viewport = viewport;
}

GUI::~GUI() {
	
}

void GUI::EnterScene(const int &scene) {
	switch (scene) {
		case SCENE_GREETING:
			Scene_Greetings();
			break;
		case SCENE_LOAD_TERE:
		case SCENE_LOAD_OBJE:
		case SCENE_LOAD_GAMEPLAY:
			Cleanup();
			Scene_Loading();
			break;
		case SCENE_OBJE:
			Cleanup();
			Scene_ObjectEditor();
			break;
		case SCENE_TERE:
			Cleanup();
			Scene_TerrainEditor();
			break;
		case SCENE_GAMEPLAY:
			Cleanup();
			Scene_Gameplay();
			break;
		default:
			break;
	}
}

void GUI::Cleanup() {
	boxes.resize(0);
	reactiveButtons.resize(0);
	texts.resize(0);
	pBars.resize(0);
	aBars.resize(0);
	aBars_text.resize(0);
	aBars_name.resize(0);
	pBar_tex.resize(0);
}

void GUI::SetViewport(const vec2 &viewport) {
	this->viewport = viewport;

	Projection = ortho(-0.5f * viewport.x, 0.5f * viewport.x, -0.5f * viewport.y, 0.5f * viewport.y, -1.0f, 1.0f);
	vec3 center = vec3(0.5f * viewport.x, 0.5f * viewport.y, 1.0f);
	vec3 direction = vec3(0, 0, -1);
	View = lookAt(center, center + direction, vec3(0, 1, 0));
	ViewProjection = Projection * View;
}

void GUI::SetEditor_Terrain(Editor_Terrain *editor_terrain) {
	this->editor_terrain = editor_terrain;
}

void GUI::SetRender_Button(Render_Button *render_button) {
	this->render_button = render_button;
}

void GUI::SetRender_Text(Render_Text *render_text) {
	this->render_text = render_text;
}

void GUI::SetRender_ProgressBar(Render_ProgressBar *render_progressBar) {
	this->render_progressBar = render_progressBar;
}

void GUI::Activate() {
	isActive = true;
}

void GUI::Deactivate() {
	isActive = false;
}

void GUI::DrawButton() {
	glDisable(GL_DEPTH_TEST);
	render_button->ActivateProgram(0);
	render_button->SetVP(ViewProjection);
	for (int i = 0; i < boxes.size(); i++) {
		if (boxes[i].isActive == false)
			continue;
		render_button->SetTexPackNum(boxes[i].texPackNum);
		render_button->SetBackground(boxes[i].tbo_background, boxes[i].color_background);
		render_button->SetBorder(boxes[i].tbo_border);
		render_button->Highlight(boxes[i].highlight);
		render_button->Draw(boxes[i].GetVao());
	}
	glEnable(GL_DEPTH_TEST);
}

void GUI::GenButtonTexture(Button &button, string filePath_background, string filePath_border) {
	glGenTextures(1, &button.tbo_background);
	glActiveTexture(GL_TEXTURE0 + button.tbo_background);
	glBindTexture(GL_TEXTURE_2D, button.tbo_background);
	render_button->LoadTexture(filePath_background.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	if (!filePath_border.empty()) {
		button.useBorder = true;
	}
	else {
		button.tbo_border = -1;
		return; 
	}

	glGenTextures(1, &button.tbo_border);
	glActiveTexture(GL_TEXTURE0 + button.tbo_border);
	glBindTexture(GL_TEXTURE_2D, button.tbo_border);
	render_button->LoadTexture(filePath_border.data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

int GUI::ButtonClickTest(const vec2 &mouseCoord) {
	vec2 newCoord(mouseCoord.x, viewport.y - mouseCoord.y);
	list<Button*>::iterator iter;
	for (iter = reactiveButtons.begin(); iter != reactiveButtons.end(); ++iter) {
		if ((*iter)->IsUnderMouse(newCoord) == true) {
			(*iter)->OnClick();
			if (!texts.empty()) {
				texts[0].DeleteMessage(0);
			}
			return (*iter)->buttonId;
		}
	}
	return -1;
}

bool GUI::ButtonUnderMouse(const vec2 &mouseCoord) {
	vec2 newCoord(mouseCoord.x, viewport.y - mouseCoord.y);
	bool found = false;
	list<Button*>::iterator iter;
	for (iter = reactiveButtons.begin(); iter != reactiveButtons.end(); ++iter) {
		if ((*iter)->IsUnderMouse(newCoord) == true) {
			(*iter)->Highlight(true);
			found = true;
		}
		else (*iter)->Highlight(false);
	}
	return found;
}

void GUI::DrawTexts() {
	glDisable(GL_DEPTH_TEST);

	render_text->ActivateProgram(0);
	render_text->SetVP(ViewProjection);
	DrawTexts(&texts);
	DrawTexts(&aBars_text);
	DrawTexts(&aBars_name);

	glEnable(GL_DEPTH_TEST);
}

void GUI::DrawTexts(vector<Text> *textPtr) {
	for (int i = 0; i < textPtr->size(); i++) {
		Text *text = &(*textPtr)[i];

		if (text->isActive == false)
			continue;
		render_text->SetTextureAtlas(text->font->tbo_font);
		// * For single text color
		//render_text->SetTextColor(text->currentString->color[text->currentCharIndex]);
		for (int j = 0; j < text->numChars; j++) {
			MsgInfo *currentMsg = &*(text->currentString);
			int currentChar = currentMsg->message[text->currentCharIndex];
			FontInfo *fontInfo = &text->font->fontInfo[currentChar];
			if (currentMsg->visible == false) {
				text->UpdateMsg();
				continue;
			}

			render_text->SetFontSize(text->fontSize);
			render_text->SetTexturePage(fontInfo->page);
			render_text->SetBoxOffset(text->origin);
			render_text->SetCharOffset(text->currentString->charOffset[text->currentCharIndex]);
			// * For various text color
			render_text->SetTextColor(text->currentString->color[text->currentCharIndex]);
			render_text->Draw(text->GetVao());
			text->UpdateMsg();
		}
	}
}

void GUI::SendText(const int &index, string msg) {
	wstring temp = CA2W(msg.data());
	texts[index].AddMessage(temp);
}

void GUI::SendText(const int &index, wstring msg) {
	texts[index].AddMessage(msg);
}

void GUI::SendChar(const int &index, const int &character) {
	texts[index].InsertChar(character);
}

void GUI::ReleaseText(const int &textBoxIndex, const int &msgIndex) {
	texts[textBoxIndex].DeleteMessage(msgIndex);
}

void GUI::HighlightBox(const int &boxIndex, const bool &tf) {
	boxes[boxIndex].Highlight(tf);
}

void GUI::DeleteChar(const int &textBoxIndex) {
	texts[textBoxIndex].DeleteChar();
}

std::wstring GUI::GetCurrentMessage(const int &textBoxIndex) {
	return texts[textBoxIndex].GetCurrentMessage();
}

void GUI::DrawProgressBar() {
	glDisable(GL_DEPTH_TEST);
	render_progressBar->ActivateProgram(0);
	render_progressBar->SetVP(ViewProjection);
	for (int i = 0; i < pBars.size(); i++) {
		ProgressBar *pBar = &pBars[i];
		if (pBar->isActive == false)
			continue;
		render_progressBar->SetProgress(pBar->progress);
		for (int j = 0; j < NUM_PB_MESH; j++) {
			render_progressBar->SetMarker(j);
			render_progressBar->SetOriginX(pBar->origin.x);
			render_progressBar->SetSizeX(pBar->size.x);
			render_progressBar->SetTexture(pBar->pBar_tex->tbo[j], pBar->color[j]);
			render_progressBar->Draw(pBar->GetVao(j));
		}
	}
	glEnable(GL_DEPTH_TEST);
}

void GUI::DrawAdjustBar() {
	glDisable(GL_DEPTH_TEST);
	render_progressBar->ActivateProgram(0);
	render_progressBar->SetVP(ViewProjection);
	for (int i = 0; i < aBars.size(); i++) {
		ProgressBar *aBar = &aBars[i];
		if (aBar->isActive == false)
			continue;
		render_progressBar->SetProgress(aBar->progress);
		for (int j = 0; j < NUM_PB_MESH; j++) {
			render_progressBar->SetMarker(j);
			render_progressBar->SetOriginX(aBar->origin.x);
			render_progressBar->SetSizeX(aBar->size.x);
			render_progressBar->SetTexture(aBar->pBar_tex->tbo[j], aBar->color[j]);
			render_progressBar->Draw(aBar->GetVao(j));
		}
	}
	glEnable(GL_DEPTH_TEST);
}

void GUI::GenProgressBarTexture(ProgressBar_Tex &pBar, string fileName_foreground, string fileName_background, 
	string fileName_effect, string fileName_cursor) {

	glGenTextures(NUM_PB_MESH, pBar.tbo);

	glActiveTexture(GL_TEXTURE0 + pBar.tbo[PB_LEFTBAR]);
	glBindTexture(GL_TEXTURE_2D, pBar.tbo[PB_LEFTBAR]);
	render_progressBar->LoadTexture((filePath_progressBar + fileName_foreground).data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 + pBar.tbo[PB_EFFECT]);
	glBindTexture(GL_TEXTURE_2D, pBar.tbo[PB_EFFECT]);
	render_progressBar->LoadTexture((filePath_progressBar + fileName_effect).data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 + pBar.tbo[PB_BACKGROUND]);
	glBindTexture(GL_TEXTURE_2D, pBar.tbo[PB_BACKGROUND]);
	render_progressBar->LoadTexture((filePath_progressBar + fileName_background).data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0 + pBar.tbo[PB_CURSOR]);
	glBindTexture(GL_TEXTURE_2D, pBar.tbo[PB_CURSOR]);
	render_progressBar->LoadTexture((filePath_progressBar + fileName_cursor).data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

bool GUI::AdjustBarClickTest(const glm::vec2 & mouseCoord) {
	vec2 newCoord(mouseCoord.x, viewport.y - mouseCoord.y);
	for (int i = 0; i < aBars.size(); i++) {
		if (aBars[i].OnCursorTest(newCoord) == true && aBars[i].isActive == true) {
			aBar_activated = &aBars[i];
			return true;
			break;
		}
	}
	return false;
}

void GUI::MoveActivatedBar(const int &moveX) {
	aBar_activated->progress = min(1, max(0, aBar_activated->progress - moveX / aBar_activated->size.x / 0.95f));
}

void GUI::AttachProperty(float *target, const float &minVal, const float &maxVal, const int &barIndex) {
	aBars[barIndex].AttachProperty(target, minVal, maxVal);
}

void GUI::DetachProperty(const int &barIndex) {
	aBars[barIndex].DetachProperty();
}

void GUI::UpdateAdjustBars() {
	for (int i = 0; i < aBars.size(); i++) {
		aBars_text[i].DeleteMessage(0);
		if (aBars[i].propertyAttached == false)
			aBars_text[i].AddMessage(L" ");
		else aBars_text[i].AddMessage(to_wstring(aBars[i].CalcProperty()));
	}
}

void GUI::Scene_Greetings() {
	fonts.resize(FONT_SIZE);
	fonts[FONT_GULIM].ReadFile("NewGulim.fnt");

	boxes.resize(GRT_SIZE);
	texts.resize(GRT_SIZE);

	boxes[GRT_MAINSCREEN].SetSize(viewport);
	boxes[GRT_MAINSCREEN].Initiate();
	boxes[GRT_MAINSCREEN].buttonId = GRT_MAINSCREEN;
	boxes[GRT_MAINSCREEN].Activate();
	boxes[GRT_MAINSCREEN].color_background = vec4(1, 1, 1, 0);
	GenButtonTexture(boxes[GRT_MAINSCREEN], "Res/Texture/Particle/noname.png", "");

	GenButtonTexture(boxes[GRT_ENT_OBJE], "Res/Texture/UI/Button/Steel.tga", "");
	for (int i = GRT_ENT_OBJE; i < GRT_SIZE; i++) {
		boxes[i].SetOrigin(vec2(50, 200 - 50 * i));
		boxes[i].SetSize(vec2(256, 30));
		boxes[i].SetColor(vec4(1, 1, 1, 0.6f));
		boxes[i].Initiate();
		boxes[i].buttonId = i;
		boxes[i].Activate();
		reactiveButtons.push_back(&boxes[i]);

		texts[i].SetSize(vec2(256, 30));
		texts[i].SetFont(&fonts[FONT_GULIM]);
		texts[i].SetOrigin(boxes[i].GetOrigin() + vec2(80, 8));
		texts[i].SetColor(vec4(0, 0, 0, 1), vec4(0, 0, 0, 1));
		texts[i].Activate();
	}
	boxes[GRT_ENT_TERE].tbo_background = boxes[GRT_ENT_OBJE].tbo_background;
	boxes[GRT_ENT_GAMEPLAY].tbo_background = boxes[GRT_ENT_OBJE].tbo_background;
	texts[GRT_ENT_OBJE].AddMessage(L"Object editor!");
	texts[GRT_ENT_TERE].AddMessage(L"Terrain editor!");
	texts[GRT_ENT_GAMEPLAY].AddMessage(L"Game Start!");
}

void GUI::Scene_Loading() {
	boxes.resize(LOAD_SIZE);
	boxes[LOAD_MAINSCREEN].SetSize(viewport);
	boxes[LOAD_MAINSCREEN].Initiate();
	boxes[LOAD_MAINSCREEN].Activate();
	GenButtonTexture(boxes[LOAD_MAINSCREEN], "Res/Pictures/bns.jpg", "");

	texts.resize(LOAD_SIZE);
	texts[LOAD_MAINSCREEN].SetOrigin(vec2(50, 100));
	texts[LOAD_MAINSCREEN].SetSize(vec2(500, 50));
	texts[LOAD_MAINSCREEN].SetFont(&fonts[FONT_GULIM]);
	texts[LOAD_MAINSCREEN].SetFontSize(1.3f);
	texts[LOAD_MAINSCREEN].SetColor(vec4(0.1f, 0.5f, 0.1f, 0.4f), vec4(0.3f, 0.4f, 0.8f, 1.0f));
	texts[LOAD_MAINSCREEN].AddMessage(L"Now Loading....");
	texts[LOAD_MAINSCREEN].Activate();
}

void GUI::Scene_ObjectEditor() {
	filePath_progressBar = "Res/Texture/UI/ProgressBar/";

	boxes.resize(OBJE_SIZE);
	texts.resize(OBJE_SIZE);
	aBar_tex.resize(1);

	// Chatter box
	texts[OBJE_CHATBOX].SetSize(vec2(300, 200));
	texts[OBJE_CHATBOX].SetFont(&fonts[FONT_GULIM]);
	texts[OBJE_CHATBOX].SetOrigin(vec2(20, 45));
	texts[OBJE_CHATBOX].AddMessage(L"======System message display box======");
	texts[OBJE_CHATBOX].AddMessage(L"한글도 지원 가능!");
	texts[OBJE_CHATBOX].SetColor(vec4(0.1f, 0.5f, 0.8f, 0.5f), vec4(0.9f, 0.6f, 0.3f, 1));
	texts[OBJE_CHATBOX].Activate();
	boxes[OBJE_CHATBOX].SetOrigin(vec2(20, 45));
	boxes[OBJE_CHATBOX].SetSize(vec2(300, 200));
	boxes[OBJE_CHATBOX].SetColor(vec4(0.1f, 0.1f, 0.1f, 0.5f));
	boxes[OBJE_CHATBOX].Initiate();
	boxes[OBJE_CHATBOX].buttonId = OBJE_CHATBOX;
	boxes[OBJE_CHATBOX].Activate();

	// Chatter box (input)
	boxes[OBJE_CHATIN].SetOrigin(vec2(20, 20));
	boxes[OBJE_CHATIN].SetSize(vec2(300, 20));
	boxes[OBJE_CHATIN].SetColor(vec4(0.1f, 0.1f, 0.1f, 0.6f));
	boxes[OBJE_CHATIN].Initiate();
	boxes[OBJE_CHATIN].buttonId = OBJE_CHATIN;
	boxes[OBJE_CHATIN].Activate();
	texts[OBJE_CHATIN].SetOrigin(vec2(20, 20));
	texts[OBJE_CHATIN].SetFont(&fonts[FONT_GULIM]);
	texts[OBJE_CHATIN].SetSize(vec2(300, 20));
	texts[OBJE_CHATIN].SetColor(vec4(1, 1, 1, 1), vec4(1, 1, 1, 1));
	texts[OBJE_CHATIN].Activate();
	
	// Save button
	boxes[OBJE_BT_SAVE].SetOrigin(vec2(30, viewport.y - 100));
	boxes[OBJE_BT_SAVE].SetSize(vec2(256, 30));
	boxes[OBJE_BT_SAVE].SetColor(vec4(153.0f / 256.0f, 204.0f / 256.0f, 255.0f / 256.0f, 0.5f));
	boxes[OBJE_BT_SAVE].Initiate();
	boxes[OBJE_BT_SAVE].buttonId = OBJE_BT_SAVE;
	boxes[OBJE_BT_SAVE].Activate();
	GenButtonTexture(boxes[OBJE_BT_SAVE], "Res/Texture/UI/Button/Steel.tga", "");
	reactiveButtons.push_back(&boxes[OBJE_BT_SAVE]);

	texts[OBJE_BT_SAVE].SetOrigin(vec2(80, viewport.y - 100 + 8));
	texts[OBJE_BT_SAVE].SetSize(vec2(256, 30));
	texts[OBJE_BT_SAVE].SetFont(&fonts[FONT_GULIM]);
	texts[OBJE_BT_SAVE].AddMessage(L"Save Transform Data");
	texts[OBJE_BT_SAVE].SetColor(vec4(0, 0, 0, 1), vec4(0, 0, 0, 1));
	texts[OBJE_BT_SAVE].Activate();

	// FPS counter
	texts[OBJE_FPS].SetOrigin(vec2(viewport.x - 100, viewport.y - 50));
	texts[OBJE_FPS].SetSize(vec2(100, 50));
	texts[OBJE_FPS].SetFont(&fonts[FONT_GULIM]);
	texts[OBJE_FPS].SetFontSize(0.8f);
	texts[OBJE_FPS].SetColor(vec4(0.1f, 0.1f, 0.1f, 1.0f), vec4(0.1f, 0.1f, 0.1f, 1.0f));
	texts[OBJE_FPS].Activate();

	// Adjust bar wrapper
	int numABars = ABAR_OBJE_SIZE;
	boxes[OBJE_ABAR_WRAP].SetOrigin(vec2(viewport.x - 210, viewport.y - 260 - 50 * numABars));
	boxes[OBJE_ABAR_WRAP].SetSize(vec2(174, 50 * numABars + 10));
	boxes[OBJE_ABAR_WRAP].SetColor(vec4(1.0f, 1.0f, 1.0f, 0.6f));
	boxes[OBJE_ABAR_WRAP].Initiate();
	boxes[OBJE_ABAR_WRAP].buttonId = OBJE_ABAR_WRAP;
	boxes[OBJE_ABAR_WRAP].Activate();


	GenProgressBarTexture(aBar_tex[0], "progressBar.png", "progressBar_bg.png",
		"progressBar_effect.png", "squareRy8.png");
	/*pBars.resize(1);
	pBars[0].SetTexture(&pBar_tex[0]);
	pBars[0].SetOrigin(600, 600);
	pBars[0].SetSize(154, 22);
	pBars[0].SetProgress(0.5f);
	pBars[0].Initiate();
	pBars[0].Activate();*/

	// Editor property adjust bar
	aBars.resize(numABars);
	aBars_name.resize(numABars);
	aBars_text.resize(numABars);

	for (int i = 0; i < aBars.size(); i++) {
		aBars[i].SetTexture(&aBar_tex[0]);
		aBars[i].SetOrigin(viewport.x - 200, viewport.y - 300 - 50 * i);
		aBars[i].SetSize(154, 22);
		aBars[i].Initiate();
		aBars[i].Activate();

		aBars_name[i].SetOrigin(aBars[i].origin + vec2(0, 25));
		aBars_name[i].SetSize(vec2(100, 50));
		aBars_name[i].SetFont(&fonts[FONT_GULIM]);
		aBars_name[i].Activate();

		aBars_text[i].SetOrigin(aBars[i].origin + vec2(80, 25));
		aBars_text[i].SetSize(vec2(100, 50));
		aBars_text[i].SetFont(&fonts[FONT_GULIM]);
		aBars_text[i].Activate();
	}
	aBars_name[ABAR_OBJE_SCALE].AddMessage(L"Scale");
	aBars_name[ABAR_OBJE_R_X].AddMessage(L"Rotation_X");
	aBars_name[ABAR_OBJE_R_Y].AddMessage(L"Rotation_Y");
	aBars_name[ABAR_OBJE_R_Z].AddMessage(L"Rotation_Z");
	aBars_name[ABAR_OBJE_P_Y].AddMessage(L"Position_Y");
}

void GUI::Scene_TerrainEditor() {
	int heightBrushSize = editor_terrain->brush_height.size();
	int textureBrushSize = editor_terrain->texture_terrain->numTerrainTex;
	boxes.resize(TERE_SIZE + heightBrushSize + textureBrushSize);
	texts.resize(TERE_SIZE + heightBrushSize + textureBrushSize);

	boxes[TERE_ABAR_WRAP].SetOrigin(viewport - vec2(210, 385));
	boxes[TERE_ABAR_WRAP].SetSize(vec2(165, 315));
	boxes[TERE_ABAR_WRAP].SetColor(vec4(1, 1, 1, 0.6f));
	boxes[TERE_ABAR_WRAP].Initiate();
	boxes[TERE_ABAR_WRAP].buttonId = TERE_ABAR_WRAP;
	boxes[TERE_ABAR_WRAP].Activate();

	boxes[TERE_BUTTON_SAVE].SetOrigin(vec2(50, viewport.y - 100));
	boxes[TERE_BUTTON_SAVE].SetSize(vec2(154, 22));
	boxes[TERE_BUTTON_SAVE].SetColor(vec4(1, 1, 1, 0.6f));
	boxes[TERE_BUTTON_SAVE].Initiate();
	boxes[TERE_BUTTON_SAVE].buttonId = TERE_BUTTON_SAVE;
	boxes[TERE_BUTTON_SAVE].Activate();
	GenButtonTexture(boxes[TERE_BUTTON_SAVE], "Res/Texture/UI/Button/Steel.tga", "");
	reactiveButtons.push_back(&boxes[TERE_BUTTON_SAVE]);
	texts[TERE_BUTTON_SAVE].SetOrigin(boxes[TERE_BUTTON_SAVE].GetOrigin() + vec2(40, 4));
	texts[TERE_BUTTON_SAVE].SetSize(vec2(100, 50));
	texts[TERE_BUTTON_SAVE].SetFont(&fonts[FONT_GULIM]);
	texts[TERE_BUTTON_SAVE].Activate();
	texts[TERE_BUTTON_SAVE].AddMessage(L"Save Data");

	for (int i = 0; i < heightBrushSize; i++) {
		boxes[TERE_SIZE + i].SetOrigin(viewport - vec2(200 - 50 * (i % 3), 230 + 50 * (i / 3)));
		boxes[TERE_SIZE + i].SetSize(vec2(45, 45));
		boxes[TERE_SIZE + i].SetColor(vec4(1, 1, 1, 0.8f));
		boxes[TERE_SIZE + i].Initiate();
		boxes[TERE_SIZE + i].buttonId = TERE_SIZE + i;
		boxes[TERE_SIZE + i].Activate();
		boxes[TERE_SIZE + i].tbo_background = editor_terrain->brush_height[i].tex_brush;
		boxes[TERE_SIZE + i].Highlight(true);
		reactiveButtons.push_back(&boxes[TERE_SIZE + i]);
	}

	int newStart = TERE_SIZE + heightBrushSize;
	for (int i = 0; i < textureBrushSize; i++) {
		boxes[newStart + i].SetOrigin(viewport - vec2(200 - 50 * (i % 3), 220 + 50 * (1 + (i / 3 + heightBrushSize / 3))));
		boxes[newStart + i].SetSize(vec2(45, 45));
		boxes[newStart + i].SetColor(vec4(1, 1, 1, 0.8f));
		boxes[newStart + i].Initiate();
		boxes[newStart + i].buttonId = newStart + i;
		boxes[newStart + i].Activate();
		boxes[newStart + i].tbo_background = editor_terrain->texture_terrain->texture_terrainPack;
		boxes[newStart + i].Highlight(true);
		boxes[newStart + i].texPackNum = i;
		reactiveButtons.push_back(&boxes[newStart + i]);
	}

	filePath_progressBar = "Res/Texture/UI/ProgressBar/";
	aBar_tex.resize(1);
	GenProgressBarTexture(aBar_tex[0], "progressBar.png", "progressBar_bg.png",
		"progressBar_effect.png", "squareRy8.png");

	aBars.resize(ABAR_TERE_SIZE);
	aBars_name.resize(ABAR_TERE_SIZE);
	aBars_text.resize(ABAR_TERE_SIZE);

	for (int i = 0; i < aBars.size(); i++) {
		aBars[i].SetTexture(&aBar_tex[0]);
		aBars[i].SetOrigin(viewport.x - 205, viewport.y - 120 - 45 * i);
		aBars[i].SetSize(154, 22);
		aBars[i].Initiate();
		aBars[i].Activate();

		aBars_name[i].SetOrigin(aBars[i].origin + vec2(0, 25));
		aBars_name[i].SetSize(vec2(100, 50));
		aBars_name[i].SetFont(&fonts[FONT_GULIM]);
		aBars_name[i].Activate();

		aBars_text[i].SetOrigin(aBars[i].origin + vec2(80, 25));
		aBars_text[i].SetSize(vec2(100, 50));
		aBars_text[i].SetFont(&fonts[FONT_GULIM]);
		aBars_text[i].Activate();
	}
	aBars_name[ABAR_TERE_BRUSHSIZE].AddMessage(L"Brush Size");
	aBars_name[ABAR_TERE_OPACITY].AddMessage(L"Opacity");
}

void GUI::Scene_Gameplay() {
	filePath_progressBar = "Res/Texture/UI/ProgressBar/";

	boxes.resize(GAME_SIZE);
	texts.resize(GAME_SIZE);

	// Chatter box
	texts[GAME_CHATBOX].SetSize(vec2(300, 200));
	texts[GAME_CHATBOX].SetFont(&fonts[FONT_GULIM]);
	texts[GAME_CHATBOX].SetOrigin(vec2(20, 45));
	texts[GAME_CHATBOX].AddMessage(L"======System message display box======");
	texts[GAME_CHATBOX].AddMessage(L"T : Display collider");
	texts[GAME_CHATBOX].AddMessage(L"1~5 : Cast magic");
	texts[GAME_CHATBOX].AddMessage(L"한글도 지원 가능!");
	texts[GAME_CHATBOX].SetColor(vec4(0.1f, 0.5f, 0.8f, 0.5f), vec4(0.9f, 0.6f, 0.3f, 1));
	texts[GAME_CHATBOX].Activate();
	boxes[GAME_CHATBOX].SetOrigin(vec2(20, 45));
	boxes[GAME_CHATBOX].SetSize(vec2(300, 200));
	boxes[GAME_CHATBOX].SetColor(vec4(0.1f, 0.1f, 0.1f, 0.5f));
	boxes[GAME_CHATBOX].Initiate();
	boxes[GAME_CHATBOX].buttonId = GAME_CHATBOX;
	boxes[GAME_CHATBOX].Activate();

	// Chatter box (input)
	boxes[GAME_CHATIN].SetOrigin(vec2(20, 20));
	boxes[GAME_CHATIN].SetSize(vec2(300, 20));
	boxes[GAME_CHATIN].SetColor(vec4(0.1f, 0.1f, 0.1f, 0.6f));
	boxes[GAME_CHATIN].Initiate();
	boxes[GAME_CHATIN].buttonId = GAME_CHATIN;
	boxes[GAME_CHATIN].Activate();
	texts[GAME_CHATIN].SetOrigin(vec2(20, 20));
	texts[GAME_CHATIN].SetFont(&fonts[FONT_GULIM]);
	texts[GAME_CHATIN].SetSize(vec2(300, 20));
	texts[GAME_CHATIN].SetColor(vec4(1, 1, 1, 1), vec4(1, 1, 1, 1));
	texts[GAME_CHATIN].Activate();

	// FPS counter
	texts[GAME_FPS].SetOrigin(vec2(viewport.x - 100, viewport.y - 50));
	texts[GAME_FPS].SetSize(vec2(100, 50));
	texts[GAME_FPS].SetFont(&fonts[FONT_GULIM]);
	texts[GAME_FPS].SetFontSize(0.8f);
	texts[GAME_FPS].SetColor(vec4(0.1f, 0.1f, 0.1f, 1.0f), vec4(0.1f, 0.1f, 0.1f, 1.0f));
	texts[GAME_FPS].Activate();
}