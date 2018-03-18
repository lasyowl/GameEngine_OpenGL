#ifndef __GAMEOBJECT_H
#define __GAMEOBJECT_H

#include "Camera.h"
#include "Render_Terrain.h"
#include "Render_Point.h"
#include "Render_Postprocess.h"
#include "Render_Sphere.h"
#include "Render_Water.h"
#include "Render_Particles.h"
#include "Render_Models_Animated.h"
#include "Render_Models_Static.h"
#include "Render_Button.h"
#include "Render_Text.h"
#include "Render_ProgressBar.h"
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
	Render_Sphere *render_sphere;
	Render_Water *render_water;
	Render_Particles *render_particles;
	Render_Models_Animated *render_models_animated;
	Render_Models_Static *render_models_static;
	Render_Button *render_button;
	Render_Text *render_text;
	Render_ProgressBar *render_progressBar;
	Terrain *terrain;
	Texture_Terrain *texture_terrain;
	Mesh_Terrain *mesh_terrain;
	Light *light;
	Framebuffer *framebuffer;
	ObjectFinder *objFinder_editor;
	GUI *gui;
	ModelTransform_Loader *loader;
	Editor_Terrain *editor_terrain;
	Postprocess *postprocess;

	ObjectInfo *pickedObj;

	std::thread thr;

private:

};
#endif