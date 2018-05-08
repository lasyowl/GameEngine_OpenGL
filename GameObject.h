#ifndef __GAMEOBJECT_H
#define __GAMEOBJECT_H

#include "Camera.h"
#include "Render_Terrain.h"
#include "Render_Point.h"
#include "Render_Postprocess.h"
#include "Render_Skymap.h"
#include "Render_Water.h"
#include "Render_Particles.h"
#include "Render_Models_Animated.h"
#include "Render_Models_Static.h"
#include "Render_Physics_CS.h"
#include "Render_Button.h"
#include "Render_Text.h"
#include "Render_ProgressBar.h"
#include "Render_Sun.h"
#include "Render_Sphere.h"
#include "Render_Line.h"
#include "Render_Collider.h"
#include "Render_HUD.h"
#include "Particle_Master.h"
#include "SpellController.h"
#include "Sun.h"
#include "Terrain.h"
#include "Texture_Terrain.h"
#include "Light.h"
#include "Controller_Character.h"
#include "Controller_Static.h"
#include "Framebuffer.h"
#include "ObjectFinder.h"
#include "GUI.h"
#include "ModelTransform_Loader.h"
#include "Editor_Terrain.h"
#include "PathFinder_Master.h"
#include "Postprocess.h"

#include <thread>

class GameObject {
public:
	GameObject();
	~GameObject();

	Camera *camera;
	Character *player;
	Controller_Character *controller_character;
	Controller_Static *controller_static;
	Render_Terrain *render_terrain;
	Render_Point *render_point;
	Render_Postprocess *render_postprocess;
	Render_Skymap *render_skymap;
	Render_Water *render_water;
	Render_Particles *render_particles;
	Render_Models_Animated *render_models_animated;
	Render_Models_Static *render_models_static;
	Render_Button *render_button;
	Render_Text *render_text;
	Render_ProgressBar *render_progressBar;
	Render_Physics_CS *render_physics;
	Render_Sun *render_sun;
	Render_Sphere *render_sphere;
	Render_Line *render_line;
	Render_Collider *render_collider;
	Render_HUD *render_hud;
	Sun *sun;
	Terrain *terrain;
	Texture_Terrain *texture_terrain;
	Mesh_Terrain *mesh_terrain;
	Light *light;
	Framebuffer *framebuffer;
	ObjectFinder *objFinder_editor;
	GUI *gui;
	ModelTransform_Loader *loader;
	Editor_Terrain *editor_terrain;
	PathFinder_Master *pathFinder;
	PathFinder_Node *currentPFNode;
	Postprocess *postprocess;
	Particle_Master *particleSystem;
	SpellController *spellController;

	ObjectInfo *pickedObj;

	std::thread thr;

private:

};
#endif