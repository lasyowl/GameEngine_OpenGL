#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "Display.h"
#include "GameObject.h"
#include "Flags_IO.h"

using namespace glm;
using namespace std;

Display* Display::me;

Display::Display() {
	me = this;
}

Display::~Display() {

}

void Display::Initiate(GameObject *gameObject, Flags_IO *flags_io) {
	this->gameObject = gameObject;
	this->flags_io = flags_io;
}

void Display::Resize(int x, int y) {

}

void Display::Draw() {

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	if (me->flags_io->sceneSwitch == false) {
		switch (me->flags_io->sceneState) {
			case SCENE_GREETING:
				me->Draw_Greetings();
				break;
			case SCENE_OBJE:
				me->Draw_ObjectEditor();
				break;
			case SCENE_TERE:
				me->Draw_TerrainEditor();
				break;
			case SCENE_GAMEPLAY:
				me->Draw_Gameplay();
				break;
			default:
				break;
		}
	}
	else {
		switch (me->flags_io->sceneState) {
		case SCENE_LOAD_GAMEPLAY:
		case SCENE_LOAD_TERE:
		case SCENE_LOAD_OBJE:
			me->Draw_Loading_ObjectEditor();
			break;
		default:
			break;
		}
	}
}

void Display::Draw_Greetings() {
	gameObject->framebuffer->BindFramebuffer(FB_POSTPROCESS0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gameObject->render_physics->Render();
	DrawGUI();
	gameObject->postprocess->Finalize(true);
	DrawPostprocess();

	glutSwapBuffers();
}

void Display::Draw_Loading_ObjectEditor() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawGUI();

	glutSwapBuffers();
}

void Display::DrawPostprocess() {
	// Brightness cutoff
	gameObject->framebuffer->BindFramebuffer(FB_POSTPROCESS1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gameObject->postprocess->RunProcess(gameObject->framebuffer->colorTex_postProcess[TEX_POSTPROCESS0], POSTPROCESS_GBLUR);
	// Gaussian blur(vertical)
	gameObject->framebuffer->BindFramebuffer(FB_POSTPROCESS2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gameObject->postprocess->RunProcess(gameObject->framebuffer->colorTex_postProcess[TEX_POSTPROCESS1], POSTPROCESS_GBLUR);
	// Gaussian blur(horizontal)
	gameObject->framebuffer->BindFramebuffer(FB_POSTPROCESS1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gameObject->postprocess->RunProcess(gameObject->framebuffer->colorTex_postProcess[TEX_POSTPROCESS2], POSTPROCESS_GBLUR);
	// Bloom process
	gameObject->framebuffer->BindFramebuffer(FB_POSTPROCESS2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gameObject->postprocess->SetAdditionalTexture(gameObject->framebuffer->colorTex_postProcess[TEX_POSTPROCESS1], PP_TEX_GBLUR);
	gameObject->postprocess->RunProcess(gameObject->framebuffer->colorTex_postProcess[TEX_POSTPROCESS0], POSTPROCESS_GBLUR);

	// Fade-in/out with contrast adjust
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int process = gameObject->postprocess->RunProcess(gameObject->framebuffer->colorTex_postProcess[TEX_POSTPROCESS2], POSTPROCESS_DEFAULT);

	// Scene termination
	if (process == PROCESS_FINISHED) {
		flags_io->sceneState = flags_io->targetScene;
		flags_io->sceneSwitch = true;
	}
}

void Display::Draw_ObjectEditor() {
	Camera *camera = gameObject->camera;
	mom += 0.5f;
	dod += 0.01f;
	ModelMatrix = rotate(mat4(1.0f), radians(dod), vec3(0, 1, 0));
	VP_sky = perspective(radians(60.0f), flags_io->viewport.x / flags_io->viewport.y, 0.1f, 2.0f) * lookAt(vec3(0.0f), camera->GetDir(), vec3(0.0f, 1.0f, 0.0f)) * ModelMatrix;

	gameObject->camera->UpdatePos();
	gameObject->render_terrain->UpdateEyePos(camera->GetPos() + camera->GetOffset());
	gameObject->controller_character->UpdateCharacters();
	gameObject->controller_static->UpdateCharacters();
	VP = camera->GetProjMatrix() * camera->GetViewMatrix();
	gameObject->render_models_animated->UpdateEyePos(camera->GetPos() + camera->GetOffset());
	gameObject->render_models_animated->UpdatePlayerPos(camera->GetPos());
	gameObject->render_models_static->UpdateEyePos(camera->GetPos() + camera->GetOffset());
	gameObject->render_models_static->UpdatePlayerPos(camera->GetPos());
	GlobalFunctions::timeElapsed = gf.CalcTimeElapsed();

	DrawOffscreen_Shadowmap();
	DrawOffscreen_Water();
	gameObject->framebuffer->BindFramebuffer(FB_POSTPROCESS0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawOnscreen();
	gameObject->postprocess->Finalize(false);
	DrawPostprocess();
	DrawGUI();

	FPSCounter_ObjectEditor();

	glutSwapBuffers();
}

void Display::Draw_TerrainEditor() {
	Camera *camera = gameObject->camera;
	camera->UpdatePos();
	VP = camera->GetProjMatrix() * camera->GetViewMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawTerrain(gameObject->terrain, VP);
	DrawGUI();

	glutSwapBuffers();
}

void Display::Draw_Gameplay() {
	Camera *camera = gameObject->camera;
	mom += 0.5f;
	dod += 0.01f;
	ModelMatrix = rotate(mat4(1.0f), radians(dod), vec3(0, 1, 0));
	VP_sky = perspective(radians(60.0f), flags_io->viewport.x / flags_io->viewport.y, 0.1f, 2.0f) * lookAt(vec3(0.0f), camera->GetDir(), vec3(0.0f, 1.0f, 0.0f)) * ModelMatrix;

	gameObject->camera->UpdatePos();
	gameObject->render_terrain->UpdateEyePos(camera->GetPos() + camera->GetOffset());
	gameObject->controller_character->UpdateCharacters();
	gameObject->controller_static->UpdateCharacters();
	VP = camera->GetProjMatrix() * camera->GetViewMatrix();
	gameObject->render_models_animated->UpdateEyePos(camera->GetPos() + camera->GetOffset());
	gameObject->render_models_animated->UpdatePlayerPos(camera->GetPos());
	gameObject->render_models_static->UpdateEyePos(camera->GetPos() + camera->GetOffset());
	gameObject->render_models_static->UpdatePlayerPos(camera->GetPos());
	GlobalFunctions::timeElapsed = gf.CalcTimeElapsed();

	DrawOffscreen_Shadowmap();
	DrawOffscreen_Water();
	gameObject->framebuffer->BindFramebuffer(FB_POSTPROCESS0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawOnscreen();
	gameObject->postprocess->Finalize(false);
	DrawPostprocess();
	DrawGUI();

	FPSCounter_Gameplay();

	glutSwapBuffers();
}

void Display::DrawGUI() {
	gameObject->gui->UpdateAdjustBars();
	gameObject->gui->DrawButton();
	gameObject->gui->DrawTexts();
	gameObject->gui->DrawProgressBar();
	gameObject->gui->DrawAdjustBar();
}

void Display::DrawOffscreen_Water() {
	Camera *camera = gameObject->camera;

	gameObject->render_water->UpdateEyePos(camera->GetPos() + camera->GetOffset());

	float surface = gameObject->render_water->GetHeight();
	vec3 reflectPos = camera->GetPos() + camera->GetOffset();
	reflectPos.y = 2 * surface - reflectPos.y;
	vec3 reflectVec = camera->GetDir();
	reflectVec.y = -reflectVec.y;
	mat4 VP_water = camera->GetProjMatrix() * lookAt(reflectPos, reflectPos + reflectVec, vec3(0, 1, 0));
	mat4 VP_sky = perspective(radians(90.0f), flags_io->viewport.x / flags_io->viewport.y, 0.1f, 2.0f) * lookAt(vec3(0.0f), reflectVec, vec3(0.0f, 1.0f, 0.0f)) * ModelMatrix;

	gameObject->framebuffer->BindFramebuffer(FB_WATER_REFLECT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CLIP_DISTANCE0);
	gameObject->render_skymap->Draw(VP_sky);
	DrawTerrain(gameObject->terrain, VP_water);
	gameObject->render_models_animated->DrawModels(VP_water, 0);
	//glEnable(GL_CULL_FACE);
	gameObject->render_models_static->Draw(VP_water);
	DrawSun();
	glDisable(GL_CLIP_DISTANCE0);

	gameObject->framebuffer->BindFramebuffer(FB_WATER_REFRACT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CLIP_DISTANCE1);
	DrawTerrain(gameObject->terrain, VP);
	gameObject->render_models_animated->DrawModels(this->VP, 0);
	gameObject->render_models_static->Draw(this->VP);
	glDisable(GL_CLIP_DISTANCE1); 
	if (flags_io->showSelectionBox == true) {
		gameObject->render_models_animated->DrawSelectionBox();
		gameObject->render_models_static->DrawSelectionBox();
	}
}

void Display::DrawOffscreen_Shadowmap() {
	CalcShadowbox();

	gameObject->framebuffer->BindFramebuffer(FB_SHADOWMAP);
	glViewport(0, 0, flags_io->viewport.x * 8, flags_io->viewport.y * 8);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gameObject->render_terrain->SetShadowMatrix(VP_shadow);
	DrawTerrain(gameObject->terrain, VP_shadow);
	gameObject->render_models_animated->DrawModels(VP_shadow, GlobalFunctions::timeElapsed);
	gameObject->render_models_static->Draw(VP_shadow);
	glViewport(0, 0, flags_io->viewport.x, flags_io->viewport.y);
}

void Display::DrawTerrain(Terrain *terrain, const mat4 &VP) {
	Camera *camera = gameObject->camera;
	vec3 refPos = camera->GetPos();
	refPos.y = 0.0f;

	for (int i = 0; i < 4; i++) {
		gameObject->terrain->child[i].FrustumTest(refPos);
	}

	Render_Terrain *render_terrain = gameObject->render_terrain;
	render_terrain->SetVAO(gameObject->mesh_terrain->GetVao());
	render_terrain->SetVP(VP);
	render_terrain->PrepareDraw();

	for (int i = 0; i < 4; i++) {
		DrawTerrain_R(&gameObject->terrain->child[i]);
	}
	render_terrain->FinishDraw();
}

void Display::DrawTerrain_R(Terrain *terrain) {
	int ibo = glm::min(terrain->lod, (int)(terrain->index.size() - 1));

	if (flags_io->sceneState == SCENE_TERE) {
		gameObject->render_terrain->SetIBO(terrain->GetIbo(ibo), terrain->numIndex[ibo]);
		gameObject->render_terrain->Draw();
	}
	else {
		if (terrain->cullFlag == DRAW_ALL) {
			//printf("%d %d %d\n", ibo, terrain->GetIbo(ibo), terrain->numIndex[ibo]);
			gameObject->render_terrain->SetIBO(terrain->GetIbo(ibo), terrain->numIndex[ibo]);
			gameObject->render_terrain->Draw();
		}
		else if (terrain->cullFlag == DRAW_CHILD) {
			for (int i = 0; i < 4; i++) {
				DrawTerrain_R(&terrain->child[i]);
			}
		}
	}
}

void Display::DrawSun() {
	vec3 camPos = gameObject->camera->GetPos() + gameObject->camera->GetOffset();
	vec3 sunPos = *gameObject->sun->position;

	vec3 newSunDir_v = camPos - sunPos;
	vec3 newSunDir_h = normalize(camPos - sunPos);
	newSunDir_v.y = 0;
	newSunDir_v = normalize(newSunDir_v);
	float angle_v = acos(dot(newSunDir_v, vec3(0, 0, -1)));
	float angle_h = acos(dot(newSunDir_h, newSunDir_v));
	if (newSunDir_v.x > 0)
		angle_v = -angle_v;
	if (newSunDir_h.y < 0)
		angle_h = -angle_h;

	mat4 trans_sun = translate(mat4(1), *gameObject->sun->position);
	mat4 rot_sun = rotate(trans_sun, angle_v, vec3(0, 1, 0));
	rot_sun = rotate(rot_sun, angle_h, vec3(1, 0, 0));
	mat4 VP_sun = perspective(radians(60.0f), flags_io->viewport.x / flags_io->viewport.y, 0.5f, 20000.0f) * gameObject->camera->GetViewMatrix();

	vec4 pos = VP_sun * vec4(sunPos - camPos, 1);
	pos /= pos.w;
	if (abs(pos.x) > 1.4f || abs(pos.y) > 1.4f)
		return;

	vec3 raycast_sun = gameObject->mesh_terrain->GetRaycastCoord_S(camPos, normalize(sunPos - camPos), 1.0f, 500.0f);
	if (raycast_sun.x != ERR_RAYCAST) {
		gameObject->render_sun->drawFlare = false;
	}
	else {
		gameObject->render_sun->drawFlare = true;
	}
	
	vec3 camDir = gameObject->camera->GetDir();
	vec3 newCamDir_v = camDir;
	vec3 newCamDir_h = camDir;
	newCamDir_v.y = 0;
	newCamDir_v = normalize(newCamDir_v);
	newCamDir_h.x = 0;
	newCamDir_h = normalize(newCamDir_h);
	newSunDir_h.x = 0;
	newSunDir_h = normalize(newSunDir_h);
	angle_v = 1 - dot(newCamDir_v, -newSunDir_v);
	angle_h = 1 - dot(newCamDir_h, -newSunDir_h);
	if (newCamDir_v.x > -newSunDir_v.x)
		angle_v = -angle_v;
	if (newCamDir_h.y > -newSunDir_h.y)
		angle_h = -angle_h;
	gameObject->render_sun->UpdateAngle(vec2(angle_v, angle_h));
	//printf("%f %f\n", angle_v, angle_h);
	gameObject->render_sun->Draw(VP_sun * rot_sun);
}

void Display::DrawOnscreen() {
	glEnable(GL_CLIP_DISTANCE2);
	Camera *camera = gameObject->camera;

	gameObject->render_skymap->Draw(VP_sky);

	DrawTerrain(gameObject->terrain, VP);
	//gameObject->render_particles->Draw(mat4(1.0f));

	gameObject->render_water->Draw(VP);
	
	gameObject->render_models_animated->DrawModels(VP, 0);
	gameObject->render_models_static->Draw(VP);
	if (flags_io->showSelectionBox == true) {
		gameObject->render_models_animated->DrawSelectionBox();
		gameObject->render_models_static->DrawSelectionBox();
	}

	////////////////////////////////////////////////////
	// Temporary -> Functionalize (Pathfinder visualization)
	////////////////////////////////////////////////////
	if (flags_io->pathFinder_state != PATHFINDER_DEACTIVATE) {
		gameObject->render_sphere->SetRadius(1.0f);
		list<PathFinder_Node>::iterator nodeIter = gameObject->pathFinder->currentGroupIter->nodes.begin();
		for (; nodeIter != gameObject->pathFinder->currentGroupIter->nodes.end(); ++nodeIter) {
			mat4 MM = translate(mat4(1), nodeIter->position + vec3(0, 1, 0));
			if (&*nodeIter == gameObject->currentPFNode)
				gameObject->render_sphere->highlight = true;
			else gameObject->render_sphere->highlight = false;
			gameObject->render_sphere->Draw(VP * MM, gameObject->pathFinder->vao, gameObject->pathFinder->mesh.index.size());
		}
		list<PathFinder_Path>::iterator pathIter = gameObject->pathFinder->currentGroupIter->paths.begin();
		for (; pathIter != gameObject->pathFinder->currentGroupIter->paths.end(); ++pathIter) {
			gameObject->render_line->Draw(pathIter->vao, VP);
		}
	}
	/////////////////////////////////////////////////////
	// Temporary -> Functionalize (Collider visualization)
	/////////////////////////////////////////////////////
	list<Character_Static>::iterator iter_col_static = gameObject->controller_static->characters.begin();
	list<Character>::iterator iter_col_active = gameObject->controller_character->characters.begin();
	list<Particles_V2>::iterator iter_col_particle = gameObject->particleSystem->particles.begin();
	//list<Character>::iterator colIter = gameObject->controller_static->characters.begin();
	if (flags_io->collider_visible == true) {
		for (; iter_col_static != gameObject->controller_static->characters.end(); ++iter_col_static) {
			gameObject->render_collider->SetVariables(VARIABLE_RADIUS, iter_col_static->collider.radius);
			mat4 MM = translate(mat4(1), iter_col_static->GetPos());
			gameObject->render_collider->Draw(VP * MM);
		}
		for (; iter_col_active != gameObject->controller_character->characters.end(); ++iter_col_active) {
			gameObject->render_collider->SetVariables(VARIABLE_RADIUS, iter_col_active->collider.radius);
			mat4 MM = translate(mat4(1), iter_col_active->GetPos());
			gameObject->render_collider->Draw(VP * MM);
		}
		for (; iter_col_particle != gameObject->particleSystem->particles.end(); ++iter_col_particle) {
			mat4 MM = translate(mat4(1), iter_col_particle->collider.center);
			gameObject->render_collider->SetVariables(VARIABLE_RADIUS, iter_col_particle->collider.radius);
			gameObject->render_collider->Draw(VP * MM);
		}
	}
	/////////////////////////////////////////////////////
	// Temporary -> Functionalize (particle system visualization)
	/////////////////////////////////////////////////////
	gameObject->render_hud->UpdateCameraPos(camera->GetPos() + camera->GetOffset());
	gameObject->render_hud->UpdateVP(VP);
	list<Character>::iterator charIter = gameObject->controller_character->characters.begin();
	for (; charIter != gameObject->controller_character->characters.end(); ++charIter) {
		gameObject->render_hud->Draw(charIter->GetPos_HUD(), charIter->hitPoint_current/charIter->hitPoint_max);
	}
	/////////////////////////////////////////////////////
	// Temporary -> Functionalize (particle system visualization)
	/////////////////////////////////////////////////////
	gameObject->particleSystem->UpdateEyePosition(camera->GetPos() + camera->GetOffset());
	gameObject->particleSystem->SortParticles(camera->GetViewMatrix());
	gameObject->particleSystem->Update();
	gameObject->particleSystem->Draw(VP);
	/////////////////////////////////////////////////////
	// Temporary -> Functionalize (particle system visualization)
	/////////////////////////////////////////////////////
	DrawSun();
	/////////////////////////////////////////////////////

	if (flags_io->raycast == true) {
		gameObject->render_point->Draw(VP);
	}
	glDisable(GL_CLIP_DISTANCE2);
}

void Display::CalcShadowbox() {
	float offset = -5.0f * gameObject->camera->GetOffsetLevel();
	float shadowDistance = glm::max(50.0f, offset);
	mat4 VP_inv = inverse(perspective(radians(60.0f), flags_io->viewport.x / flags_io->viewport.y, 0.5f, shadowDistance) * gameObject->camera->GetViewMatrix());
	shadowBox[NLD] = VP_inv * vec4(-1, -1, -1, 1);
	shadowBox[NLU] = VP_inv * vec4(-1, 1, -1, 1);
	shadowBox[NRD] = VP_inv * vec4(1, -1, -1, 1);
	shadowBox[NRU] = VP_inv * vec4(1, 1, -1, 1);
	shadowBox[FLD] = VP_inv * vec4(-1, -1, 1, 1);
	shadowBox[FLU] = VP_inv * vec4(-1, 1, 1, 1);
	shadowBox[FRD] = VP_inv * vec4(1, -1, 1, 1);
	shadowBox[FRU] = VP_inv * vec4(1, 1, 1, 1);
	
	vec3 lightDir = gameObject->light->direction[0];
	lightDir = normalize(lightDir);

	mat4 V_shadow = lookAt(gameObject->player->GetPos(), gameObject->player->GetPos() + lightDir, vec3(0, 1, 0));

	for (int i = 0; i < 8; i++) {
		shadowBox[i] /= shadowBox[i].w;
		shadowBox[i] = V_shadow * shadowBox[i];
	}
	float minX = shadowBox[NLD].x;
	float maxX = shadowBox[NLD].x;
	float minY = shadowBox[NLD].y;
	float maxY = shadowBox[NLD].y;
	float minZ = shadowBox[NLD].z;
	float maxZ = shadowBox[NLD].z;
	for (int i = 0; i < 8; i++) {
		minX = glm::min(minX, shadowBox[i].x);
		maxX = glm::max(maxX, shadowBox[i].x);
		minY = glm::min(minY, shadowBox[i].y);
		maxY = glm::max(maxY, shadowBox[i].y);
		minZ = glm::min(minZ, shadowBox[i].z);
		maxZ = glm::max(maxZ, shadowBox[i].z);
	}
	vec4 cameraDir(gameObject->player->GetPos() + gameObject->camera->GetDir(), 1);
	cameraDir = V_shadow * cameraDir;
	float width = maxX - minX + 50.0f * abs(dot(vec3(cameraDir), vec3(1, 0, 0)));
	float height = maxY - minY + 50.0f * abs(dot(vec3(cameraDir), vec3(0, 1, 0)));
	vec4 center((maxX + minX) * 0.5f, (maxY + minY) * 0.5f, (maxZ + minZ) * 0.5f, 1);
	
	VP_shadow = ortho(center.x - width, center.x + width, center.y - height, center.y + height, -800.0f, 800.0f) * V_shadow;
}

void Display::FPSCounter_ObjectEditor() {
	static float renderTime;
	static float count;
	renderTime += gf.End();
	count++;
	if (renderTime > 1000) {
		gameObject->gui->ReleaseText(OBJE_FPS, 0);
		glFinish();
		char buffer[10];

		_itoa_s((int)(count), buffer, 10);
		string str0, str1;
		str0 = buffer;
		str1 = "FPS";

		gameObject->gui->SendText(OBJE_FPS, str0 + str1);
		count = 0;
		renderTime = 0;
	}
	gf.Start();
}

void Display::FPSCounter_Gameplay() {
	static float renderTime;
	static float count;
	renderTime += gf.End();
	count++;
	if (renderTime > 1000) {
		gameObject->gui->ReleaseText(GAME_FPS, 0);
		glFinish();
		char buffer[10];

		_itoa_s((int)(count), buffer, 10);
		string str0, str1;
		str0 = buffer;
		str1 = "FPS";

		gameObject->gui->SendText(GAME_FPS, str0 + str1);
		count = 0;
		renderTime = 0;
	}
	gf.Start();
}