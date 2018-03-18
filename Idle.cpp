#include <glm/gtc/matrix_transform.hpp>

#include "Idle.h"
#include "Flags_IO.h"
#include "GameObject.h"

using namespace glm;
using namespace std;

Idle* Idle::me;

Idle::Idle() {
	me = this;
}

Idle::~Idle() {

}

void Idle::Initiate(GameObject *gameObject, Flags_IO *flags_io) {
	this->gameObject = gameObject;
	this->flags_io = flags_io;
}

void Idle::Update() {
	switch (me->flags_io->sceneState) {
		case SCENE_GREETING:
			me->Update_Greetings();
			break;
		case SCENE_OBJE:
			me->Update_ObjectEditor();
			break;
		case SCENE_TERE:
			me->Update_TerrainEditor();
			break;
		default:
			break;
	}
}

void Idle::Update_Greetings() {
	
}

void Idle::Update_ObjectEditor() {
	me->PlayerMove();
}

void Idle::Update_TerrainEditor() {
	me->CameraMove();
}

#define move 5.4f
void Idle::PlayerMove() {
	Character *character = gameObject->player;
	Camera *camera = gameObject->camera;

	//character->Update();

	if (flags_io->isPressed[KEYBOARD_a])
		character->Move(-character->GetRightDir(), move);
	if (flags_io->isPressed[KEYBOARD_s])
		character->Move(-character->GetDir(), move);
	if (flags_io->isPressed[KEYBOARD_d])
		character->Move(character->GetRightDir(), move);
	if (flags_io->isPressed[KEYBOARD_w])
		character->Move(character->GetDir(), move);
	if (flags_io->isPressed[MOUSE_LEFT] && !flags_io->isPressed[MOUSE_RIGHT] && !flags_io->locked[CAMERA]) {
		camera->Rotate(SEQ_ONE, flags_io->pointerMoved.x);
		camera->Rotate(SEQ_TWO, flags_io->pointerMoved.y);
		flags_io->pointerMoved = vec2(0);
	}
	if (flags_io->isPressed[MOUSE_RIGHT]) {
		camera->Rotate(SEQ_ONE, flags_io->pointerMoved.x);
		camera->Rotate(SEQ_TWO, flags_io->pointerMoved.y);
		character->Rotate(SEQ_ONE, flags_io->pointerMoved.x);

		vec3 dir = camera->GetDir();
		dir.y = 0;
		normalize(dir);
		character->SetDir(dir);

		flags_io->pointerMoved = vec2(0);
	}
	
}

void Idle::CameraMove() {
	Camera *camera = gameObject->camera;

	vec3 camDir = gameObject->camera->GetDir();
	camDir.y = 0.0f;
	camDir = normalize(camDir);
	vec3 camRightDir = gameObject->camera->GetRightDir();
	camRightDir.y = 0.0f;
	camRightDir = normalize(camRightDir);

	if (flags_io->isPressed[KEYBOARD_a])
		camera->Move(-camRightDir, move);
	if (flags_io->isPressed[KEYBOARD_s])
		camera->Move(-camDir, move);
	if (flags_io->isPressed[KEYBOARD_d])
		camera->Move(camRightDir, move);
	if (flags_io->isPressed[KEYBOARD_w])
		camera->Move(camDir, move);
	if (flags_io->isPressed[KEYBOARD_space])
		camera->Move(vec3(0, 1, 0), move);
	if (flags_io->isPressed[KEYBOARD_x])
		camera->Move(vec3(0, -1, 0), move);

	if (flags_io->isPressed[MOUSE_RIGHT]) {
		camera->Rotate(SEQ_ONE, flags_io->pointerMoved.x);
		camera->Rotate(SEQ_TWO, flags_io->pointerMoved.y);
		flags_io->pointerMoved = vec2(0);
	}

	if (flags_io->isPressed[MOUSE_LEFT]) {
		if (flags_io->status_func[CLICKING_BUTTON] == false && flags_io->status_func[MOVING_BAR] == false) {
			if(gameObject->editor_terrain->textureMode == false)
				gameObject->editor_terrain->AdjustHeight(flags_io->raycastCoord, true);
			else gameObject->editor_terrain->AdjustTexture(flags_io->raycastCoord);
		}
	}
	if (flags_io->isPressed[MOUSE_MIDDLE]) {
		gameObject->editor_terrain->AdjustHeight(flags_io->raycastCoord, false);
	}
}