#include "GameManager.h"
#include "GameObject.h"

using namespace glm;
using namespace std;

GameManager* GameManager::me;

GameManager::GameManager() : sequence(0) {
	me = this;
}

GameManager::~GameManager() {
	delete gameObject;
}
#define TERRAIN_WIDTH 2048
#define RADIUS 200.0f
void GameManager::Initiate(const float &viewportX, const float &viewportY) {
	flags_io = new Flags_IO;
	flags_io->viewport = vec2(viewportX, viewportY);

	gameObject = new GameObject;

	gameObject->framebuffer = new Framebuffer;
	gameObject->light = new Light;
	gameObject->mesh_terrain = new Mesh_Terrain;
	gameObject->terrain = new Terrain;
	gameObject->texture_terrain = new Texture_Terrain;
	gameObject->render_terrain = new Render_Terrain;
	gameObject->render_point = new Render_Point;
	gameObject->render_postprocess = new Render_Postprocess;
	gameObject->render_sphere = new Render_Sphere;
	gameObject->render_water = new Render_Water;
	gameObject->render_particles = new Render_Particles;
	gameObject->render_models_animated = new Render_Models_Animated;
	gameObject->render_models_static = new Render_Models_Static;
	gameObject->render_button = new Render_Button;
	gameObject->render_text = new Render_Text;
	gameObject->render_progressBar = new Render_ProgressBar;
	gameObject->editor_terrain = new Editor_Terrain;
	gameObject->objFinder_editor = new ObjectFinder;
	gameObject->controller_character = new Controller_Character;
	gameObject->controller_static = new Controller_Static;
	gameObject->loader = new ModelTransform_Loader;
	gameObject->camera = new Camera(flags_io->viewport);
	gameObject->gui = new GUI(flags_io->viewport);
	gameObject->editor_terrain = new Editor_Terrain;
	gameObject->postprocess = new Postprocess;

	// Set framebuffer
	gameObject->framebuffer->SetViewport(&flags_io->viewport);
	gameObject->framebuffer->InitFramebuffer();

	// Set postprocess
	gameObject->postprocess->SetViewport(&flags_io->viewport);
	gameObject->postprocess->SetRenderer(gameObject->render_postprocess);
	gameObject->postprocess->Initiate();
	gameObject->render_postprocess->Initiate();

	// Set GUI
	gameObject->gui->SetViewport(flags_io->viewport);
	gameObject->gui->SetRender_Button(gameObject->render_button);
	gameObject->gui->SetRender_Text(gameObject->render_text);
	gameObject->gui->SetRender_ProgressBar(gameObject->render_progressBar);

	// Initiate IO callback functions
	keyboard.Initiate(gameObject, flags_io);
	mouse.Initiate(gameObject, flags_io);
	display.Initiate(gameObject, flags_io);
	idle.Initiate(gameObject, flags_io);
	menu.Initiate(flags_io);

	EnterScene(SCENE_GREETING);
}

void GameManager::IdleFunc() {
	if (me->flags_io->sceneSwitch == true) {
		me->flags_io->sceneSwitch = false;
		me->EnterScene(me->flags_io->sceneState);
	}
	me->idle.Update();
	me->display.Draw();
}

void GameManager::EnterScene(const int &scene) {
	switch (scene) {
		case SCENE_GREETING:
			Scene_Greetings();
			break;
		case SCENE_LOAD_OBJE:
			Scene_LoadObjectEditor();
			break;
		case SCENE_LOAD_TERE:
			Scene_LoadTerrainEditor();
			break;
		default:
			break;
	}
}

void GameManager::MenuStatusFunc(int status, int x, int y) {
	switch (status) {
		case GLUT_MENU_IN_USE:
			me->flags_io->menuInUse = true;
			break;
		case GLUT_MENU_NOT_IN_USE:
			me->flags_io->menuInUse = false;
			break;
		default:
			break;
	}
}

void GameManager::Scene_Greetings() {
	gameObject->gui->EnterScene(SCENE_GREETING);
}

void GameManager::Scene_LoadObjectEditor() {
	switch (flags_io->loadSequence) {
		case LOADING_GUI:
			gameObject->gui->EnterScene(SCENE_LOAD_OBJE);
			break;
		case LOADING_LIGHT:
			// Set light
			gameObject->light->AddLight(glm::vec3(350.0f, 100.0f, -400.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.1f, 0.2f, 1.0f), 20000.0f);
			gameObject->light->AddLight(glm::vec3(550.0f, 100.0f, -300.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.1f, 0.2f, 1.0f), 20000.0f);
			break;
		case LOADING_MESH_TERRAIN:
			// Set terrain mesh
			gameObject->mesh_terrain->SetDimension(TERRAIN_WIDTH, vec2(0));
			gameObject->mesh_terrain->Initiate();
			break;
		case LOADING_TERRAIN:
			// Set terrain index
			gameObject->terrain->SetRadius(RADIUS);
			gameObject->terrain->SetMesh(gameObject->mesh_terrain);
			gameObject->terrain->Initiate(TERRAIN_WIDTH, LD, vec2(0), TERRAIN_WIDTH);
			gameObject->terrain->GenObject();
			break;
		case LOADING_TEXTURE_TERRAIN:
			// Set terrain texture
			gameObject->texture_terrain->SetMesh(gameObject->mesh_terrain);
			gameObject->texture_terrain->GenTextures();
			break;
		case LOADING_RENDERER_TERRAIN:
			// Set terrain renderer
			gameObject->render_terrain->SetTerrainWidth(TERRAIN_WIDTH);
			gameObject->render_terrain->SetTexture_Terrain(gameObject->texture_terrain);
			gameObject->render_terrain->SetFramebuffer(gameObject->framebuffer);
			gameObject->render_terrain->SetLight(gameObject->light);
			gameObject->render_terrain->Initiate();
			break;
		case LOADING_RENDERER_POINT:
			// Set point renderer
			gameObject->render_point->Initiate();
			break;
		case LOADING_RENDERER_SPHERE:
			// Set sphere renderer
			gameObject->render_sphere->Initiate();
			gameObject->render_sphere->EnableSkyMode();
			break;
		case LOADING_RENDERER_WATER:
			// Set water renderer
			gameObject->render_water->SetFramebuffer(gameObject->framebuffer);
			gameObject->render_water->SetLight(gameObject->light);
			gameObject->render_water->InitWaterRenderer();
			gameObject->render_water->AddWater(vec2(TERRAIN_WIDTH / 2), vec3(TERRAIN_WIDTH / 2, 5.0f, -TERRAIN_WIDTH / 2));
			break;
		case LOADING_RENDERER_ANIMATED:
			// Set animated model renderer
			gameObject->render_models_animated->SetDrawRadius(RADIUS);
			gameObject->render_models_animated->SetFramebuffer(gameObject->framebuffer);
			gameObject->render_models_animated->SetLight(gameObject->light);
			gameObject->render_models_animated->InitModelRenderer();
			break;
		case LOADING_RENDERER_STATIC:
			// Set static model renderer
			gameObject->render_models_static->SetDrawRadius(RADIUS);
			gameObject->render_models_static->SetLight(gameObject->light);
			gameObject->render_models_static->InitModelRenderer();
			break;
		case LOADING_CONTROLLER_ANIMATED:
			// Set animated character controller
			gameObject->controller_character->InitController(gameObject->render_models_animated, gameObject->mesh_terrain);
			break;
		case LOADING_CONTROLLER_STATIC:
			// Set static character controller
			gameObject->controller_static->InitController(gameObject->render_models_static, gameObject->mesh_terrain);
			break;
		case LOADING_MODEL_TRANSFORM_LOADER:
			// Set model transform loader
			gameObject->objFinder_editor->SetRenderer(gameObject->render_models_static, gameObject->render_models_animated);
			gameObject->loader->ReadFile("Res/Models/ModelTransform.mdl");
			for (int i = 0; i < gameObject->loader->numObjects; i++) {
				switch (gameObject->loader->objInfo[i].tag) {
				case 0:
					gameObject->player = gameObject->controller_character->AddCharacter(gameObject->loader->objInfo[i], gameObject->objFinder_editor);
					break;
				case 1:
					break;
				case 2:
					gameObject->controller_static->AddCharacter(gameObject->loader->objInfo[i], gameObject->objFinder_editor);
					break;
				default:
					break;
				}
			}
			break;
		case LOADING_GLUT_MENU:
			// Set glut menu
			menu.SetObjFinder(gameObject->objFinder_editor);
			menu.SetDatabase(gameObject->loader);
			menu.SetControllers(gameObject->controller_character, gameObject->controller_static);
			menu.CreateEditorMenu();
			break;
		case LOADING_CAMERA:
			// Set camera
			gameObject->camera->SetReference(gameObject->player);
			gameObject->camera->SetPov(THIRD_PERSON);
			break;
		case LOADING_FINISHED:
			// Set GUI
			gameObject->gui->EnterScene(SCENE_OBJE);
			flags_io->sceneState = SCENE_OBJE;
			break;
		default:
			break;
	}
	if(flags_io->loadSequence != LOADING_FINISHED)
		flags_io->sceneSwitch = true;
	flags_io->loadSequence++;
}

void GameManager::Scene_LoadTerrainEditor() {
	switch (flags_io->loadSequence) {
		case LOADING_GUI:
			gameObject->gui->EnterScene(SCENE_LOAD_TERE);
			break;
		case LOADING_LIGHT:
			// Set light
			gameObject->light->AddLight(glm::vec3(350.0f, 100.0f, -400.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.1f, 0.2f, 1.0f), 20000.0f);
			break;
		case LOADING_MESH_TERRAIN:
			// Set terrain mesh
			gameObject->mesh_terrain->SetDimension(TERRAIN_WIDTH, vec2(0));
			gameObject->mesh_terrain->Initiate();
			break;
		case LOADING_TERRAIN:
			// Set terrain index
			gameObject->terrain->SetRadius(RADIUS);
			gameObject->terrain->SetMesh(gameObject->mesh_terrain);
			gameObject->terrain->Initiate(TERRAIN_WIDTH, LD, vec2(0), TERRAIN_WIDTH);
			gameObject->terrain->GenObject();
			break;
		case LOADING_TEXTURE_TERRAIN:
			// Set terrain texture
			gameObject->texture_terrain->SetMesh(gameObject->mesh_terrain);
			gameObject->texture_terrain->GenTextures();
			break;
		case LOADING_RENDERER_TERRAIN:
			// Set terrain renderer
			gameObject->render_terrain->EnableMode(FAST_MODE);
			gameObject->render_terrain->SetTerrainWidth(TERRAIN_WIDTH);
			gameObject->render_terrain->SetTexture_Terrain(gameObject->texture_terrain);
			gameObject->render_terrain->SetFramebuffer(gameObject->framebuffer);
			gameObject->render_terrain->SetLight(gameObject->light);
			gameObject->render_terrain->Initiate();
			break;
		case LOADING_EDITOR_TERRAIN:
			// Set terrain editor
			gameObject->editor_terrain->SetViewport(&flags_io->viewport);
			gameObject->editor_terrain->SetTargetMesh(gameObject->mesh_terrain);
			gameObject->editor_terrain->SetTexture_Terrrain(gameObject->texture_terrain);
			gameObject->editor_terrain->Initiate();
			break;
		case LOADING_RENDERER_POINT:
			// Set point renderer
			gameObject->render_point->Initiate();
			break;
		case LOADING_CAMERA:
			// Set camera
			gameObject->camera->SetPov(STAND_ALONE);
			break;
		case LOADING_FINISHED:
			// Set GUI
			gameObject->gui->SetEditor_Terrain(gameObject->editor_terrain);
			gameObject->gui->EnterScene(SCENE_TERE);
			gameObject->gui->AttachProperty(&gameObject->editor_terrain->brushSize, 0, 200, ABAR_TERE_BRUSHSIZE);
			gameObject->gui->AttachProperty(&gameObject->editor_terrain->opacity, 0.001f, 1.0f, ABAR_TERE_OPACITY);
			flags_io->sceneState = SCENE_TERE;
			break;
		default:
			break;
	}
	if (flags_io->loadSequence != LOADING_FINISHED)
		flags_io->sceneSwitch = true;
	flags_io->loadSequence++;
}

void GameManager::CleanUp() {
	delete gameObject->camera;
	gameObject->player = nullptr;
	delete gameObject->loader;
	delete gameObject->controller_static;
	delete gameObject->controller_character;
	delete gameObject->render_models_static;
	delete gameObject->render_models_animated;
	delete gameObject->render_water;
	delete gameObject->render_sphere;
	delete gameObject->render_point;
	delete gameObject->render_terrain;
	delete gameObject->terrain;
	delete gameObject->mesh_terrain;
	delete gameObject->light;
}