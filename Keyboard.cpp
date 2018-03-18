#include <GL/glew.h>
#include <gl/freeglut.h>

#include "Keyboard.h"
#include "GameObject.h"
#include "Flags_IO.h"
#include <atlstr.h>

using namespace std;

Keyboard* Keyboard::me;

Keyboard::Keyboard() {
	me = this;
}

Keyboard::~Keyboard() {

}

void Keyboard::Initiate(GameObject *gameObject, Flags_IO *flags_io) {
	this->gameObject = gameObject;
	this->flags_io = flags_io;
}

void Keyboard::KeyboardUp(unsigned char key, int x, int y) {
	if (me->flags_io->isTyping == true)
		return;

	switch (me->flags_io->sceneState) {
		case SCENE_GREETING:
			me->KeyboardUp_Greetings(key, x, y);
			break;
		case SCENE_OBJE:
			me->KeyboardUp_ObjectEditor(key, x, y);
			break;
		case SCENE_TERE:
			me->KeyboardUp_TerrainEditor(key, x, y);
			break;
		default:
			break;
	}
}

void Keyboard::KeyboardPress(unsigned char key, int x, int y) {
	switch (me->flags_io->sceneState) {
		case SCENE_GREETING:
			me->KeyboardPress_Greetings(key, x, y);
			break;
		case SCENE_OBJE:
			me->KeyboardPress_ObjectEditor(key, x, y);
			break;
		case SCENE_TERE:
			me->KeyboardPress_TerrainEditor(key, x, y);
			break;
		default:
			break;
	}
}

void Keyboard::SpecialKeyUp(int key, int x, int y) {
	switch (me->flags_io->sceneState) {
	case SCENE_GREETING:
		me->SpecialKeyUp_Greetings(key, x, y);
		break;
	case SCENE_OBJE:
		me->SpecialKeyUp_ObjectEditor(key, x, y);
		break;
	case SCENE_TERE:
		me->SpecialKeyUp_TerrainEditor(key, x, y);
		break;
	default:
		break;
	}
}

void Keyboard::SpecialKeyPress(int key, int x, int y) {
	switch (me->flags_io->sceneState) {
	case SCENE_GREETING:
		me->SpecialKeyPress_Greetings(key, x, y);
		break;
	case SCENE_OBJE:
		me->SpecialKeyPress_ObjectEditor(key, x, y);
		break;
	case SCENE_TERE:
		me->SpecialKeyPress_TerrainEditor(key, x, y);
		break;
	default:
		break;
	}
}

void Keyboard::KeyboardUp_Greetings(const unsigned char &key, const int &x, const int &y) {

}

void Keyboard::KeyboardUp_ObjectEditor(const unsigned char &key, const int &x, const int &y) {
	switch (key) {
	case 'w':
		flags_io->isPressed[KEYBOARD_w] = false;
		if (MoveKeyReleased() && gameObject->player->animState == STATE_MOVE)
			gameObject->player->SetState(STATE_IDLE);
		break;
	case 's':
		flags_io->isPressed[KEYBOARD_s] = false;
		if (MoveKeyReleased() && gameObject->player->animState == STATE_MOVE)
			gameObject->player->SetState(STATE_IDLE);
		break;
	case 'a':
		flags_io->isPressed[KEYBOARD_a] = false;
		if (MoveKeyReleased() && gameObject->player->animState == STATE_MOVE)
			gameObject->player->SetState(STATE_IDLE);
		break;
	case 'd':
		flags_io->isPressed[KEYBOARD_d] = false;
		if (MoveKeyReleased() && gameObject->player->animState == STATE_MOVE)
			gameObject->player->SetState(STATE_IDLE);
		break;
	case 'v':
		flags_io->isPressed[KEYBOARD_v] = false;
		gameObject->camera->SwitchPov();
		break;
	case 'x':
		flags_io->isPressed[KEYBOARD_x] = false;
		flags_io->showSelectionBox = !flags_io->showSelectionBox;
		//controller_character->DeleteCharacter();
		break;
	case 'c':
		flags_io->isPressed[KEYBOARD_c] = false;
		flags_io->raycast = !flags_io->raycast;
		break;
	case 'r':
		flags_io->isPressed[KEYBOARD_r] = false;
		break;
	case 't':
		flags_io->isPressed[KEYBOARD_t] = false;
		break;
	case '.':
		flags_io->mouse_movable = !flags_io->mouse_movable;
		break;
	case 32: // space bar in ascii
		flags_io->isPressed[KEYBOARD_space] = false;
		//physics->SwitchFlag();
		break;
	case 127: // delete key in ascii
		if (gameObject->pickedObj != nullptr) {
			if (gameObject->pickedObj->tag == 2) {
				gameObject->controller_static->DeleteCharacter((Character_Static*)(gameObject->pickedObj->character));
				gameObject->pickedObj->IsSelected(false);
				gameObject->pickedObj = nullptr;
			}
			else if (gameObject->pickedObj->tag == 0) {
				gameObject->controller_character->DeleteCharacter(gameObject->pickedObj->character);
				gameObject->pickedObj->IsSelected(false);
				gameObject->pickedObj = nullptr;
			}
		}
		break;
	case 27: // esc in ascii
		break;
	default:
		break;
	}
}

void Keyboard::KeyboardUp_TerrainEditor(const unsigned char &key, const int &x, const int &y) {
	switch (key) {
	case 'w':
		flags_io->isPressed[KEYBOARD_w] = false;
		break;
	case 's':
		flags_io->isPressed[KEYBOARD_s] = false;
		break;
	case 'a':
		flags_io->isPressed[KEYBOARD_a] = false;
		break;
	case 'd':
		flags_io->isPressed[KEYBOARD_d] = false;
		break;
	case 'x':
		flags_io->isPressed[KEYBOARD_x] = false;
		break;
	case 't':
		gameObject->editor_terrain->textureMode = !gameObject->editor_terrain->textureMode;
		break;
	case 32: // space bar in ascii
		flags_io->isPressed[KEYBOARD_space] = false;
		break;
	default:
		break;
	}
}

void Keyboard::KeyboardPress_Greetings(const unsigned char &key, const int &x, const int &y) {

}

void Keyboard::KeyboardPress_ObjectEditor(const unsigned char &key, const int &x, const int &y) {
	if (key > 127 && flags_io->isHangul == false) {
		flags_io->isHangul = true;
		flags_io->hangulTemp = key;
		return;
	}
	else if (flags_io->isHangul == true) {
		string str_temp;
		str_temp.resize(2);
		str_temp[0] = flags_io->hangulTemp - 256;
		str_temp[1] = key - 256;
		wstring wStr_temp = CA2W(str_temp.data());
		if (flags_io->isTyping == true)
			PressTyping(wStr_temp[0]);
		flags_io->isHangul = false;
	}
	else {
		if (flags_io->isTyping == false)
			PressNotTyping(key);
		else PressTyping(key);
	}
}

void Keyboard::KeyboardPress_TerrainEditor(const unsigned char &key, const int &x, const int &y) {
	switch (key) {
		case 'w':
			flags_io->isPressed[KEYBOARD_w] = true;
			break;
		case 's':
			flags_io->isPressed[KEYBOARD_s] = true;
			break;
		case 'a':
			flags_io->isPressed[KEYBOARD_a] = true;
			break;
		case 'd':
			flags_io->isPressed[KEYBOARD_d] = true;
			break;
		case 'x':
			flags_io->isPressed[KEYBOARD_x] = true;
			break;
		case 32: // space bar in ascii
			flags_io->isPressed[KEYBOARD_space] = true;
			break;
		case 27: // esc in ascii
			glutLeaveMainLoop();
			break;
		default:
			break;
	}
}

void Keyboard::SpecialKeyUp_Greetings(const int &key, const int &x, const int &y) {

}

void Keyboard::SpecialKeyUp_ObjectEditor(const int &key, const int &x, const int &y) {
	switch (key) {
	case GLUT_KEY_F10:
		if (flags_io->activateMenu == false) {
			flags_io->activateMenu = true;
			glutSetMenu(1);
			glutAttachMenu(GLUT_RIGHT_BUTTON);
			gameObject->gui->SendText(OBJE_CHATBOX, "Activated editor menu");
			//printf("Activated editor menu\n");
		}
		else {
			if (flags_io->menuInUse == true)
				break;
			flags_io->activateMenu = false;
			glutDetachMenu(GLUT_RIGHT_BUTTON);
			gameObject->gui->SendText(OBJE_CHATBOX, "Deactivated editor menu");
			//printf("Deactivated editor menu\n");
		}
		break;
	default:
		break;
	}
}

void Keyboard::SpecialKeyUp_TerrainEditor(const int &key, const int &x, const int &y) {

}

void Keyboard::SpecialKeyPress_Greetings(const int &key, const int &x, const int &y) {

}

void Keyboard::SpecialKeyPress_ObjectEditor(const int &key, const int &x, const int &y) {

}

void Keyboard::SpecialKeyPress_TerrainEditor(const int &key, const int &x, const int &y) {

}

bool Keyboard::MoveKeyReleased() {
	if (flags_io->isPressed[KEYBOARD_w] == false && flags_io->isPressed[KEYBOARD_s] == false && flags_io->isPressed[KEYBOARD_d] == false && flags_io->isPressed[KEYBOARD_a] == false)
		return true;
	else return false;
}

void Keyboard::PressTyping(const unsigned int &key) {
	if (key > 31 && key < 127 && flags_io->isHangul == false) {
		string str; str = str + (char)key;
		gameObject->gui->SendChar(OBJE_CHATIN, key);
	}
	else if (flags_io->isHangul == true) {
		gameObject->gui->SendChar(OBJE_CHATIN, key);
	}
	wstring temp;
	switch (key) {
	case 8: // backspace in ascii
		gameObject->gui->DeleteChar(OBJE_CHATIN);
		break;
	case 27: // esc in ascii
		flags_io->isTyping = !flags_io->isTyping;
		gameObject->gui->HighlightBox(OBJE_CHATIN, flags_io->isTyping);
		gameObject->gui->ReleaseText(OBJE_CHATIN, 0);
		break;
	case 13: // enter in ascii
		flags_io->isTyping = !flags_io->isTyping;
		gameObject->gui->HighlightBox(OBJE_CHATIN, flags_io->isTyping);
		temp = gameObject->gui->GetCurrentMessage(OBJE_CHATIN);
		if (!temp.empty())
			gameObject->gui->SendText(OBJE_CHATBOX, temp);
		gameObject->gui->ReleaseText(OBJE_CHATIN, 0);
		break;
	default:
		break;
	}
}

void Keyboard::PressNotTyping(const unsigned char &key) {
	switch (key) {
	case 'w':
		if (flags_io->IsMoving() == false)
			gameObject->player->SetState(STATE_WALK);
		flags_io->isPressed[KEYBOARD_w] = true;
		break;
	case 's':
		if (flags_io->IsMoving() == false)
			gameObject->player->SetState(STATE_WALK);
		flags_io->isPressed[KEYBOARD_s] = true;
		break;
	case 'a':
		if (flags_io->IsMoving() == false)
			gameObject->player->SetState(STATE_WALK);
		flags_io->isPressed[KEYBOARD_a] = true;
		break;
	case 'd':
		if (flags_io->IsMoving() == false)
			gameObject->player->SetState(STATE_WALK);
		flags_io->isPressed[KEYBOARD_d] = true;
		break;
	case 'x':
		flags_io->isPressed[KEYBOARD_x] = true;
		break;
	case 'c':
		flags_io->isPressed[KEYBOARD_c] = true;
		break;
	case 'r':
		if (flags_io->isPressed[KEYBOARD_r] == false)
			gameObject->player->SetState(STATE_ATTACK);
		flags_io->isPressed[KEYBOARD_r] = true;
		break;
	case 't':
		flags_io->isPressed[KEYBOARD_t] = true;
		break;
	case 32: // space bar in ascii
		if (flags_io->isPressed[KEYBOARD_space] == false && gameObject->player->IsJumping() == false)
			gameObject->player->SetState(STATE_JUMP);
		flags_io->isPressed[KEYBOARD_space] = true;
		break;
	case 27: // esc in ascii
		glutLeaveMainLoop();
		break;
	case 13: // enter in ascii
		flags_io->isTyping = !flags_io->isTyping;
		gameObject->gui->HighlightBox(OBJE_CHATIN, flags_io->isTyping);
		break;
	default:
		break;
	}
}
