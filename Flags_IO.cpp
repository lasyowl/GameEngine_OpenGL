#include "Flags_IO.h"

using namespace glm;

Flags_IO::Flags_IO() : raycast(false), showSelectionBox(false), allKeyboardReleased(0), activateMenu(false), menuInUse(false),
isTyping(false), isHangul(false), mouse_movable(false), sceneState(SCENE_GREETING), sceneSwitch(false), process(0.0f),
loadSequence(0), targetScene(-1) {
	isPressed.resize(100, false);
	locked.resize(100, false);
	status_func.resize(100, false);
	pointerMoved = vec2(0);
	mouseCoord = vec2(0);
}

Flags_IO::~Flags_IO() {

}

bool Flags_IO::IsMoving() {
	return isPressed[KEYBOARD_a] + isPressed[KEYBOARD_s] + isPressed[KEYBOARD_w] + isPressed[KEYBOARD_d];
}
