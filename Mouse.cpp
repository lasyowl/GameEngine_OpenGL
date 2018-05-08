#include <GL/glew.h>
#include <gl/freeglut.h>

#include "Mouse.h"
#include "GameObject.h"
#include "Flags_IO.h"

using namespace glm;
using namespace std;

Mouse* Mouse::me;

Mouse::Mouse() {
	me = this;
}

Mouse::~Mouse() {

}

void Mouse::Initiate(GameObject *gameObject, Flags_IO *flags_io) {
	this->gameObject = gameObject;
	this->flags_io = flags_io;
	this->gui = gameObject->gui;
	this->camera = gameObject->camera;
}

void Mouse::MousePress(int button, int state, int x, int y) {
	switch (me->flags_io->sceneState) {
		case SCENE_GREETING:
			me->MousePress_Greetings(button, state, x, y);
			break;
		case SCENE_OBJE:
			me->MousePress_ObjectEditor(button, state, x, y);
			break;
		case SCENE_TERE:
			me->MousePress_TerrainEditor(button, state, x, y);
			break;
		case SCENE_GAMEPLAY:
			me->MousePress_Gameplay(button, state, x, y);
			break;
		default:
			break;
	}
	
}

void Mouse::MouseMoveActive(int x, int y) {
	switch (me->flags_io->sceneState) {
		case SCENE_GREETING:
			me->MouseMoveActive_Greetings(x, y);
			break;
		case SCENE_OBJE:
			me->MouseMoveActive_ObjectEditor(x, y);
			break;
		case SCENE_TERE:
			me->MouseMoveActive_TerrainEditor(x, y);
			break;
		case SCENE_GAMEPLAY:
			me->MouseMoveActive_Gameplay(x, y);
			break;
		default:
			break;
	}
}

void Mouse::MouseMovePassive(int x, int y) {
	switch (me->flags_io->sceneState) {
	case SCENE_GREETING:
		me->MouseMovePassive_Greetings(x, y);
		break;
	case SCENE_OBJE:
		me->MouseMovePassive_ObjectEditor(x, y);
		break;
	case SCENE_TERE:
		me->MouseMovePassive_TerrainEditor(x, y);
		break;
	case SCENE_GAMEPLAY:
		me->MouseMovePassive_Gameplay(x, y);
		break;
	default:
		break;
	}
}

void Mouse::MouseWheel(int wheel, int direction, int x, int y) {
	Camera *camera = me->gameObject->camera;
	if (direction > 0) { // Zoom-in
		camera->AdjOffset(CAMERA_OFFSET);
	}
	else { // Zoom-out
		camera->AdjOffset(-CAMERA_OFFSET);
	}
}

void Mouse::MousePress_Greetings(const int &button, const int &state, const int &x, const int &y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {

		}
		else {
			flags_io->mouseCoord = vec2(x, y);
			int buttonInfo = gui->ButtonClickTest(flags_io->mouseCoord);
			switch(buttonInfo){
				case GRT_ENT_OBJE:
					printf("Enter object editor\n");
					gameObject->postprocess->SetProcessState(POSTPROCESS_DEFAULT, PROCESS_FINISHING);
					flags_io->targetScene = SCENE_LOAD_OBJE;
					break;
				case GRT_ENT_TERE:
					printf("Enter terrain editor\n");
					gameObject->postprocess->SetProcessState(POSTPROCESS_DEFAULT, PROCESS_FINISHING);
					flags_io->targetScene = SCENE_LOAD_TERE;
					break;
				case GRT_ENT_GAMEPLAY:
					printf("Enter gameplay\n");
					gameObject->postprocess->SetProcessState(POSTPROCESS_DEFAULT, PROCESS_FINISHING);
					flags_io->targetScene = SCENE_LOAD_GAMEPLAY;
					break;
				default:
					break;
			}
		}
	default:
		break;
	}
}

void Mouse::MousePress_ObjectEditor(const int &button, const int &state, const int &x, const int &y) {
	switch (button) {
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			flags_io->isPressed[MOUSE_RIGHT] = true;
			flags_io->mouseCoord = vec2(x, y);
		}
		else {
			flags_io->isPressed[MOUSE_RIGHT] = false;
			flags_io->pointerMoved = vec2(0);
		}
		break;
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			flags_io->isPressed[MOUSE_LEFT] = true;
			flags_io->mouseCoord = vec2(x, y);

			// Left click priority
			// 1. Pathfinder
			if (flags_io->pathFinder_state != PATHFINDER_DEACTIVATE) {
				flags_io->raycastCoord = gameObject->mesh_terrain->GetRaycastCoord_S(camera->GetPos() + camera->GetOffset(), camera->GetMouseWorldCoord(vec2(x, y)), 0.5f, DEFAULT_RAYCAST_RANGE);
				
				PathFinder_Node *node;
				switch (flags_io->pathFinder_state) {
				case PATHFINDER_ADD_NODE:
					if (flags_io->raycastCoord.x != ERR_RAYCAST) {
						gameObject->pathFinder->CreateNode(flags_io->raycastCoord);
					}
					break;
				case PATHFINDER_SELECT_NODE:
					node = gameObject->pathFinder->FindNearestNode(flags_io->raycastCoord);
					if (gameObject->currentPFNode != nullptr) {
						gameObject->pathFinder->LinkNode(gameObject->currentPFNode, node);
					}
					gameObject->currentPFNode = node;
					/*if(node != nullptr)
						printf("%f %f %f\n", node->position.x, node->position.y, node->position.z);
					else {
						printf("Failed to find node\n");
					}*/
					break;
				case PATHFINDER_PATHFIND:
					if (flags_io->raycastCoord.x != ERR_RAYCAST) {
						vector<PathFinder_Node*> path = gameObject->pathFinder->FindPath(gameObject->player, flags_io->raycastCoord);
						printf("=======================\n");
						for (int i = 0; i < path.size(); i++) {
							printf("%d\n", path[i]->index);
						}
						gameObject->player->ClearPath();
						gameObject->player->ReservePath(path);
					}
					break;
				default:
					break;
				}
				
			}
			// 2. GUI - Save
			else if (gui->ButtonClickTest(flags_io->mouseCoord) == OBJE_BT_SAVE) {
				gameObject->loader->BeginSaving("Res/Models/ModelTransform.mdl", gameObject->objFinder_editor->objectList.size());
				list<ObjectInfo*>::iterator objIter = gameObject->objFinder_editor->objectList.begin();
				for (; objIter != gameObject->objFinder_editor->objectList.end(); ++objIter) {
					gameObject->loader->SaveData(**objIter);
				}
				gameObject->loader->FinishSaving();
			}
			// 3. GUI - Property adjust
			else if (gui->AdjustBarClickTest(flags_io->mouseCoord) == true) {
				flags_io->locked[CAMERA] = true;
				flags_io->status_func[MOVING_BAR] = true;
			}
			// 4. Object release
			else if (gameObject->pickedObj != nullptr) {
				gameObject->pickedObj->IsSelected(false);
				gameObject->pickedObj = nullptr;
				gameObject->gui->DetachProperty(OBJE_CHATBOX);
				gui->SendText(OBJE_CHATBOX, "Object released");
			}
			// 5. Object picking
			else {
				gameObject->pickedObj = gameObject->objFinder_editor->FindObjectRaycast(camera->GetPos() + camera->GetOffset(), camera->GetMouseWorldCoord(vec2(x, y)), 0.5f, DEFAULT_RAYCAST_RANGE);
				if (gameObject->pickedObj != nullptr) {
					gameObject->gui->AttachProperty(&gameObject->pickedObj->character->scale, 0.0001f, 1.0f, ABAR_OBJE_SCALE);
					gameObject->gui->AttachProperty(&gameObject->pickedObj->character->preRotation.x, 0.0f, 360.0f, ABAR_OBJE_R_X);
					gameObject->gui->AttachProperty(&gameObject->pickedObj->character->preRotation.y, 0.0f, 360.0f, ABAR_OBJE_R_Y);
					gameObject->gui->AttachProperty(&gameObject->pickedObj->character->preRotation.z, 0.0f, 360.0f, ABAR_OBJE_R_Z);
					gameObject->gui->AttachProperty(&gameObject->pickedObj->character->position.y, 0.0f, 300.0f, ABAR_OBJE_P_Y);
				}
			}
		}
		else {
			flags_io->isPressed[MOUSE_LEFT] = false;
			flags_io->pointerMoved = vec2(0);
			if (flags_io->status_func[MOVING_BAR] == true) {
				gui->aBar_activated = nullptr;
				flags_io->locked[CAMERA] = false;
				flags_io->status_func[MOVING_BAR] == false;
			}
		}
	default:
		break;
	}
}

void Mouse::MousePress_TerrainEditor(const int &button, const int &state, const int &x, const int &y) {
	switch (button) {
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			flags_io->isPressed[MOUSE_RIGHT] = true;
			flags_io->mouseCoord = vec2(x, y);
		}
		else {
			flags_io->isPressed[MOUSE_RIGHT] = false;
			flags_io->pointerMoved = vec2(0);
		}
		break;
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			flags_io->isPressed[MOUSE_LEFT] = true;
			flags_io->mouseCoord = vec2(x, y);
			int button = gui->ButtonClickTest(flags_io->mouseCoord);
			if (button > 0) {
				if (button == TERE_BUTTON_SAVE) {
					gameObject->editor_terrain->SaveAlphamap("Res/Heightmap/Alpha_save.png");
					gameObject->editor_terrain->SaveHeightmap("Res/Heightmap/Height_save.png");
					return;
				}
				flags_io->status_func[CLICKING_BUTTON] = true;
				int textureOffset = gameObject->editor_terrain->brush_height.size() + TERE_SIZE;
				if (button >= textureOffset) {
					gameObject->editor_terrain->SetTexture(button - textureOffset);
				}
				else {
					gameObject->editor_terrain->SetBrush(button - TERE_SIZE);
				}
			}
			if (gui->AdjustBarClickTest(flags_io->mouseCoord) == true) {
				flags_io->status_func[MOVING_BAR] = true;
			}
		}
		else {
			flags_io->isPressed[MOUSE_LEFT] = false;
			flags_io->pointerMoved = vec2(0);
			flags_io->status_func[CLICKING_BUTTON] = false;
			if (flags_io->status_func[MOVING_BAR] == true) {
				gui->aBar_activated = nullptr;
				flags_io->status_func[MOVING_BAR] = false;
			}
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			flags_io->isPressed[MOUSE_MIDDLE] = true;
			flags_io->mouseCoord = vec2(x, y);
		}
		else {
			flags_io->isPressed[MOUSE_MIDDLE] = false;
			flags_io->pointerMoved = vec2(0);
		}
	default:
		break;
	}
}

void Mouse::MousePress_Gameplay(const int &button, const int &state, const int &x, const int &y) {
	switch (button) {
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			flags_io->isPressed[MOUSE_RIGHT] = true;
			flags_io->mouseCoord = vec2(x, y);
		}
		else {
			flags_io->isPressed[MOUSE_RIGHT] = false;
			flags_io->pointerMoved = vec2(0);
		}
		break;
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			flags_io->isPressed[MOUSE_LEFT] = true;
			flags_io->mouseCoord = vec2(x, y);

			// Left click priority
			// 1. Pathfinder
			if (flags_io->pathFinder_state != PATHFINDER_DEACTIVATE) {
				flags_io->raycastCoord = gameObject->mesh_terrain->GetRaycastCoord_S(camera->GetPos() + camera->GetOffset(), camera->GetMouseWorldCoord(vec2(x, y)), 0.5f, DEFAULT_RAYCAST_RANGE);

				PathFinder_Node *node;
				switch (flags_io->pathFinder_state) {
				case PATHFINDER_ADD_NODE:
					if (flags_io->raycastCoord.x != ERR_RAYCAST) {
						gameObject->pathFinder->CreateNode(flags_io->raycastCoord);
					}
					break;
				case PATHFINDER_SELECT_NODE:
					node = gameObject->pathFinder->FindNearestNode(flags_io->raycastCoord);
					if (gameObject->currentPFNode != nullptr) {
						gameObject->pathFinder->LinkNode(gameObject->currentPFNode, node);
					}
					gameObject->currentPFNode = node;
					/*if(node != nullptr)
					printf("%f %f %f\n", node->position.x, node->position.y, node->position.z);
					else {
					printf("Failed to find node\n");
					}*/
					break;
				case PATHFINDER_PATHFIND:
					if (flags_io->raycastCoord.x != ERR_RAYCAST) {
						vector<PathFinder_Node*> path = gameObject->pathFinder->FindPath(gameObject->player, flags_io->raycastCoord);
						printf("=======================\n");
						for (int i = 0; i < path.size(); i++) {
							printf("%d\n", path[i]->index);
						}
						gameObject->player->ClearPath();
						gameObject->player->ReservePath(path);
					}
					break;
				default:
					break;
				}

			}
			else {
				gameObject->pickedObj = gameObject->objFinder_editor->FindObjectRaycast(camera->GetPos() + camera->GetOffset(), camera->GetMouseWorldCoord(vec2(x, y)), 0.5f, DEFAULT_RAYCAST_RANGE);
				if (gameObject->pickedObj != nullptr) {
					printf("Picked object\n");
				}
			}
		}
		else {
			flags_io->isPressed[MOUSE_LEFT] = false;
			flags_io->pointerMoved = vec2(0);
			if (flags_io->status_func[MOVING_BAR] == true) {
				gui->aBar_activated = nullptr;
				flags_io->locked[CAMERA] = false;
				flags_io->status_func[MOVING_BAR] == false;
			}
		}
	default:
		break;
	}
}

void Mouse::MouseMoveActive_Greetings(const int &x, const int &y) {

}

void Mouse::MouseMoveActive_ObjectEditor(const int &x, const int &y) {
	vec2 newCoord = vec2(x, y);
	flags_io->pointerMoved = flags_io->mouseCoord - newCoord;
	flags_io->mouseCoord = newCoord;
	if (gameObject->gui->aBar_activated != nullptr) {
		gameObject->gui->MoveActivatedBar(flags_io->pointerMoved.x);
		if (gameObject->pickedObj != nullptr) {
			gameObject->pickedObj->character->GenPreRotMatrix();
		}
	}
}

void Mouse::MouseMoveActive_TerrainEditor(const int &x, const int &y) {
	vec2 newCoord = vec2(x, y);
	flags_io->pointerMoved = flags_io->mouseCoord - newCoord;
	flags_io->mouseCoord = newCoord;
	flags_io->raycastCoord = gameObject->mesh_terrain->GetRaycastCoord_S(camera->GetPos() + camera->GetOffset(), camera->GetMouseWorldCoord(vec2(x, y)), 0.5f,  3 * DEFAULT_RAYCAST_RANGE);
	if (gameObject->gui->aBar_activated != nullptr) {
		gameObject->gui->MoveActivatedBar(flags_io->pointerMoved.x);
	}
}

void Mouse::MouseMoveActive_Gameplay(const int &x, const int &y) {
	vec2 newCoord = vec2(x, y);
	flags_io->pointerMoved = flags_io->mouseCoord - newCoord;
	flags_io->mouseCoord = newCoord;
}

void Mouse::MouseMovePassive_Greetings(const int &x, const int &y) {
	gameObject->gui->ButtonUnderMouse(vec2(x, y));
}

void Mouse::MouseMovePassive_ObjectEditor(const int &x, const int &y) {
	gameObject->gui->ButtonUnderMouse(vec2(x, y));
	Camera *camera = gameObject->camera;
	flags_io->raycastCoord = gameObject->mesh_terrain->GetRaycastCoord_S(camera->GetPos() + camera->GetOffset(), camera->GetMouseWorldCoord(vec2(x, y)), 0.5f, DEFAULT_RAYCAST_RANGE);

	if (flags_io->raycastCoord.x != ERR_RAYCAST) {
		gameObject->render_point->UpdatePoint(flags_io->raycastCoord);
	}
	if (gameObject->pickedObj != nullptr) {
		if (flags_io->mouse_movable == true) {
			gameObject->pickedObj->character->SetXZPos(flags_io->raycastCoord.x, flags_io->raycastCoord.z);
		}
	}
}

void Mouse::MouseMovePassive_TerrainEditor(const int &x, const int &y) {
	gameObject->gui->ButtonUnderMouse(vec2(x, y));
	flags_io->raycastCoord = gameObject->mesh_terrain->GetRaycastCoord_S(camera->GetPos() + camera->GetOffset(), camera->GetMouseWorldCoord(vec2(x, y)), 0.5f, 3 * DEFAULT_RAYCAST_RANGE);
}

void Mouse::MouseMovePassive_Gameplay(const int &x, const int &y) {
	gameObject->gui->ButtonUnderMouse(vec2(x, y));
	Camera *camera = gameObject->camera;
	flags_io->raycastCoord = gameObject->mesh_terrain->GetRaycastCoord_S(camera->GetPos() + camera->GetOffset(), camera->GetMouseWorldCoord(vec2(x, y)), 0.5f, DEFAULT_RAYCAST_RANGE);

	if (flags_io->raycastCoord.x != ERR_RAYCAST) {
		gameObject->render_point->UpdatePoint(flags_io->raycastCoord);
	}
}
