#ifndef __FLAGS_IO_H
#define __FLAGS_IO_H

#include <vector>
#include <glm/glm.hpp>

enum Enum_Button {
	KEYBOARD_a, KEYBOARD_b, KEYBOARD_c, KEYBOARD_d, KEYBOARD_e, KEYBOARD_f, KEYBOARD_g,
	KEYBOARD_h, KEYBOARD_i, KEYBOARD_j, KEYBOARD_k, KEYBOARD_l, KEYBOARD_m, KEYBOARD_n,
	KEYBOARD_o, KEYBOARD_p, KEYBOARD_q, KEYBOARD_r, KEYBOARD_s, KEYBOARD_t, KEYBOARD_u,
	KEYBOARD_v, KEYBOARD_w, KEYBOARD_x, KEYBOARD_y, KEYBOARD_z, KEYBOARD_space = 32,
	MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE, MOUSE_WHEELUP, MOUSE_WHEELDOWN,
	KEYBOARD_SHIFT_L, KEYBOARD_SHIFT_R
};

enum Lock_Function {
	CAMERA
};

enum Status_Function {
	MOVING_BAR, CLICKING_BUTTON
};

enum PathFinder_State {
	PATHFINDER_DEACTIVATE, PATHFINDER_ADD_NODE, PATHFINDER_SELECT_NODE, PATHFINDER_PATHFIND, PATHFINDER_NUM_STATES
};

enum SceneState { SCENE_CLOSING, SCENE_GREETING, SCENE_LOAD_OBJE, SCENE_LOAD_TERE, SCENE_LOAD_GAMEPLAY, SCENE_TERE, SCENE_OBJE, SCENE_GAMEPLAY };

enum LoadState { LOADING_GUI, LOADING_LIGHT, LOADING_MESH_TERRAIN,
	LOADING_TERRAIN, LOADING_TEXTURE_TERRAIN, LOADING_RENDERER_SUN, LOADING_RENDERER_TERRAIN, LOADING_EDITOR_TERRAIN, LOADING_RENDERER_POINT, LOADING_RENDERER_SPHERE, 
	LOADING_RENDERER_WATER, LOADING_RENDERER_ANIMATED, LOADING_RENDERER_STATIC, LOADING_CONTROLLER_ANIMATED,
	LOADING_CONTROLLER_STATIC, LOADING_MODEL_TRANSFORM_LOADER, LOADING_GLUT_MENU, LOADING_CAMERA, LOADING_PARTICLESYS, LOADING_FINISHED };

class Flags_IO {
public:
	Flags_IO();
	~Flags_IO();

	bool IsMoving();

	std::vector<bool> status_func;
	std::vector<bool> isPressed;
	std::vector<bool> locked;
	int allKeyboardReleased;
	glm::vec2 mouseCoord;
	glm::vec2 pointerMoved;
	glm::vec3 raycastCoord;
	bool mouse_movable;
	bool raycast;
	bool showSelectionBox;
	bool activateMenu;
	bool menuInUse;
	bool isTyping;
	bool isHangul;
	int hangulTemp;
	glm::vec2 viewport;
	int sceneState;
	bool sceneSwitch;
	float process;
	int loadSequence;
	int targetScene;
	int pathFinder_state;
	bool collider_visible;

private:

};
#endif