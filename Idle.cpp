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
		case SCENE_GAMEPLAY:
			me->Update_Gameplay();
			break;
		default:
			break;
	}
}

void Idle::Update_Greetings() {
	
}

void Idle::Update_ObjectEditor() {
	me->PlayerMove();

	gameObject->spellController->Update();
}

void Idle::Update_TerrainEditor() {
	me->CameraMove();
}

void Idle::Update_Gameplay() {
	me->PlayerMove();

	gameObject->spellController->Update();

	/////////////////////////////////////////////////////
	// Temporary -> Functionalize (Collision detection)
	/////////////////////////////////////////////////////
	list<Character_Static>::iterator staticIter = gameObject->controller_static->characters.begin();
	list<Character>::iterator charIter = gameObject->controller_character->characters.begin();
	list<Particles_V2>::iterator particleIter = gameObject->particleSystem->particles.begin();
	int flag = NO_COLLISION;
	int flag_particle = NO_COLLISION;
	int count = 0;
	for (; particleIter != gameObject->particleSystem->particles.end(); ++particleIter) {
		for (; charIter != gameObject->controller_character->characters.end(); ++charIter) {
			if (particleIter->initProperty.owner == &*charIter) {
				continue;
			}
			if (particleIter->collider.CollisionTest(&charIter->collider) == ENTER_COLLISION) {
				if (particleIter->collider.taskDone == false) {
					charIter->Damage(20.0f);
					printf("Object %d : HP %f left\n", charIter->iter->charIndex, charIter->hitPoint_current);
					particleIter->collider.taskDone = true;
					if (charIter->hitPoint_current <= 0) {
						if (&*charIter == gameObject->player) {
							return;
						}
						gameObject->controller_character->DeleteCharacter(&*charIter);
						gameObject->particleSystem->particles.erase(particleIter);
						--particleIter;
						break;
					}
				}
			}
		}
		charIter = gameObject->controller_character->characters.begin();
	}
	
	for (; staticIter != gameObject->controller_static->characters.end(); ++staticIter) {
		flag = gameObject->player->collider.CollisionTest(&staticIter->collider);

		if (flag == ENTER_COLLISION) {
			printf("ENTER COLLISION : %d\n", count);
		}
		else if (flag == EXIT_COLLISION) {
			printf("EXIT COLLISION : %d\n", count);
		}
		count++;
	}

	/////////////////////////////////////////////////////
	// Temporary -> Functionalize (Enemy -> Player recognization)
	/////////////////////////////////////////////////////
	auto iter_enemy = gameObject->controller_character->characters.begin();
	++iter_enemy;
	for (; iter_enemy != gameObject->controller_character->characters.end(); ++iter_enemy) {
		vec3 enemyDir = gameObject->player->GetPos() - iter_enemy->GetPos();
		vec3 norm = normalize(enemyDir);
		float sca = enemyDir.x / norm.x;
		if (sca < iter_enemy->recogDist) {
			iter_enemy->SetDir(-norm);
			if (iter_enemy->animState == STATE_IDLE) {
				SpellSet spellSet;
				Particle_Property tempProperty;
				iter_enemy->SetState(STATE_CASTING);
				tempProperty = { 5, 1, 1, 100, iter_enemy->GetPos(), iter_enemy->GetDir(), 0.0f, 1.0f, 2000, 1000, 2.0f, true, &*iter_enemy };
				spellSet.target = gameObject->player;
				spellSet.owner = &*iter_enemy;
				spellSet.spell.push_back(tempProperty);
				spellSet.timeRemain.push_back(2000);
				tempProperty = { 3, 100, 1, 100, iter_enemy->GetPos(), iter_enemy->GetDir(), rand() % 10 + 1.0f, 1.0f, 5000, 1000, 2.0f, false, &*iter_enemy };
				spellSet.spell.push_back(tempProperty);
				spellSet.timeRemain.push_back(5000);
				gameObject->spellController->RegisterSpell(spellSet);
				/*if (iter_enemy->animState != STATE_MOVE) {
				iter_enemy->SetState(STATE_WALK);
				}
				iter_enemy->Move(norm);*/
			}
		}
		else {
			if (iter_enemy->animState != STATE_IDLE) {
				iter_enemy->SetState(STATE_IDLE);
			}
		}
	}
	/////////////////////////////////////////////////////
	// Temporary -> Functionalize (Character state)
	/////////////////////////////////////////////////////
	if (gameObject->player->animState == STATE_JUMP) {
		if (gameObject->player->animDuration < 0) {
			if (flags_io->IsMoving() == true) {
				if (flags_io->isPressed[KEYBOARD_SHIFT_L] == true) {
					gameObject->player->SetState(STATE_RUN);
				}
				else {
					gameObject->player->SetState(STATE_WALK);
				}
			}
		}
		else {
			gameObject->player->animDuration -= GlobalFunctions::timeElapsed;
		}
	}
	/////////////////////////////////////////////////////
}

void Idle::PlayerMove() {
	Character *character = gameObject->player;
	Camera *camera = gameObject->camera;

	//character->Update();

	if (flags_io->isPressed[KEYBOARD_a]) {
		character->Move(-character->GetRightDir());
	}
	if (flags_io->isPressed[KEYBOARD_s]) {
		character->Move(-character->GetDir());
	}
	if (flags_io->isPressed[KEYBOARD_d]) {
		character->Move(character->GetRightDir());
	}
	if (flags_io->isPressed[KEYBOARD_w]) {
		character->Move(character->GetDir());
	}
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
		camera->Move(-camRightDir, gameObject->player->speed);
	if (flags_io->isPressed[KEYBOARD_s])
		camera->Move(-camDir, gameObject->player->speed);
	if (flags_io->isPressed[KEYBOARD_d])
		camera->Move(camRightDir, gameObject->player->speed);
	if (flags_io->isPressed[KEYBOARD_w])
		camera->Move(camDir, gameObject->player->speed);
	if (flags_io->isPressed[KEYBOARD_space])
		camera->Move(vec3(0, 1, 0), gameObject->player->speed);
	if (flags_io->isPressed[KEYBOARD_x])
		camera->Move(vec3(0, -1, 0), gameObject->player->speed);

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